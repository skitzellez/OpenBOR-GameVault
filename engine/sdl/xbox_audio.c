/*
 * Copyright (c) 2026 Skitzelles - part of Game Vault, https://github.com/skitzellez/xbox-game-vault
 * Sound on the Xbox: SDL's audio device never finishes opening in Xbox (UWP) apps, so the engine's mixer plays
 * through XAudio2. A feeder thread asks update_sample for the next block whenever XAudio2 has room.
 */
#define COBJMACROS
#include <windows.h>
#include <objbase.h>
#include <xaudio2.h>
#include <string.h>

/* windows.h and the engine's headers both define DWORD and friends: the engine's functions are declared by hand */
extern void update_sample(unsigned char *buf, int size);
int SB_playstart(int bits, int samplerate);
void SB_playstop(void);
void SB_lock_audio(void);
void SB_unlock_audio(void);
void SB_lock_audio_direct(void);
void SB_unlock_audio_direct(void);
void SB_setvolume(char dev, char volume);
void SB_updatevolume(int volume);

#define XB_BLOCK_FRAMES 1024
#define XB_BLOCKS 6
#define XB_QUEUED 3

typedef HRESULT (WINAPI *xaudio2_create_fn)(IXAudio2 **, UINT32, XAUDIO2_PROCESSOR);

static IXAudio2 *xb_engine;
static IXAudio2MasteringVoice *xb_master;
static IXAudio2SourceVoice *xb_voice;
static HANDLE xb_thread;
static volatile LONG xb_run;
static int xb_bits;
static int started;
static CRITICAL_SECTION xb_lock;
static int xb_lock_ready;
static short xb_blocks[XB_BLOCKS][XB_BLOCK_FRAMES * 2];
static int xb_raw[XB_BLOCK_FRAMES * 2];

static void xb_fill(short *out)
{
	int i, n = XB_BLOCK_FRAMES * 2;
	EnterCriticalSection(&xb_lock);
	if(xb_bits == 16)
	{
		update_sample((unsigned char *)out, n * 2);
	}
	else if(xb_bits == 8)
	{
		unsigned char *raw = (unsigned char *)xb_raw;
		update_sample(raw, n);
		for(i = 0; i < n; i++) out[i] = (short)(((int)raw[i] - 128) << 8);
	}
	else
	{
		update_sample((unsigned char *)xb_raw, n * 4);
		for(i = 0; i < n; i++) out[i] = (short)(xb_raw[i] >> 16);
	}
	LeaveCriticalSection(&xb_lock);
}

static DWORD WINAPI xb_feeder(LPVOID unused)
{
	int next = 0;
	(void)unused;
	while(xb_run)
	{
		XAUDIO2_VOICE_STATE state;
		XAUDIO2_BUFFER buffer;
		IXAudio2SourceVoice_GetState(xb_voice, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if(state.BuffersQueued >= XB_QUEUED)
		{
			Sleep(2);
			continue;
		}
		xb_fill(xb_blocks[next]);
		memset(&buffer, 0, sizeof(buffer));
		buffer.AudioBytes = sizeof(xb_blocks[next]);
		buffer.pAudioData = (const BYTE *)xb_blocks[next];
		IXAudio2SourceVoice_SubmitSourceBuffer(xb_voice, &buffer, NULL);
		next = (next + 1) % XB_BLOCKS;
	}
	return 0;
}

int SB_playstart(int bits, int samplerate)
{
	HMODULE lib;
	xaudio2_create_fn create;
	WAVEFORMATEX format;

	if(started) return 1;
	if(bits != 8 && bits != 16 && bits != 24) return 0;
	if(!xb_lock_ready)
	{
		InitializeCriticalSection(&xb_lock);
		xb_lock_ready = 1;
	}
	lib = LoadLibraryExW(L"xaudio2_9.dll", NULL, 0);
	create = lib ? (xaudio2_create_fn)(void *)GetProcAddress(lib, "XAudio2Create") : NULL;
	if(!create) return 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(!xb_engine)
	{
		if(FAILED(create(&xb_engine, 0, XAUDIO2_DEFAULT_PROCESSOR))) return 0;
		if(FAILED(IXAudio2_CreateMasteringVoice(xb_engine, &xb_master, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL, AudioCategory_GameEffects))) return 0;
	}
	memset(&format, 0, sizeof(format));
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = samplerate;
	format.wBitsPerSample = 16;
	format.nBlockAlign = 4;
	format.nAvgBytesPerSec = samplerate * 4;
	if(FAILED(IXAudio2_CreateSourceVoice(xb_engine, &xb_voice, &format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL))) return 0;
	xb_bits = bits;
	IXAudio2SourceVoice_Start(xb_voice, 0, XAUDIO2_COMMIT_NOW);
	xb_run = 1;
	xb_thread = CreateThread(NULL, 0, xb_feeder, NULL, 0, NULL);
	if(!xb_thread)
	{
		xb_run = 0;
		IXAudio2SourceVoice_DestroyVoice(xb_voice);
		xb_voice = NULL;
		return 0;
	}
	SetThreadPriority(xb_thread, THREAD_PRIORITY_HIGHEST);
	started = 1;
	return 1;
}

void SB_playstop(void)
{
	if(!started) return;
	xb_run = 0;
	WaitForSingleObject(xb_thread, 2000);
	CloseHandle(xb_thread);
	xb_thread = NULL;
	IXAudio2SourceVoice_Stop(xb_voice, 0, XAUDIO2_COMMIT_NOW);
	IXAudio2SourceVoice_DestroyVoice(xb_voice);
	xb_voice = NULL;
	started = 0;
}

void SB_lock_audio(void) { SB_lock_audio_direct(); }
void SB_unlock_audio(void) { SB_unlock_audio_direct(); }
void SB_lock_audio_direct(void) { if(xb_lock_ready) EnterCriticalSection(&xb_lock); }
void SB_unlock_audio_direct(void) { if(xb_lock_ready) LeaveCriticalSection(&xb_lock); }
void SB_setvolume(char dev, char volume) { (void)dev; (void)volume; }
void SB_updatevolume(int volume) { (void)volume; }
