/*
 * Copyright (c) 2026 Skitzelles - part of Game Vault, https://github.com/skitzellez/xbox-game-vault
 * OpenBOR for Game Vault (Xbox, UWP Dev Mode). OpenBOR itself: BSD-3-Clause, see LICENSE.
 *
 * The Xbox entry point. SDL's WinRT backend (our SDL3 for UWP) owns the app's CoreWindow, so the program
 * starts in WinMain and SDL_RunApp calls OpenBOR's main on the UI thread (OpenBOR uses SDL2: sdl2-compat over SDL3).
 * The game to play comes from Game Vault: "<publisher folder>\games\openbor\launch.txt" holds the .pak's
 * file name inside "games\openbor\Paks" (Game Vault copies the .pak there first: an Xbox app cannot open a
 * USB file with plain C file calls).
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* SDL3.dll is the library under sdl2-compat: looked up by name so the two libraries' same-named functions never mix */
typedef int (*xb_main_fn)(int, char **);
typedef int (*xb_runapp_fn)(int, char **, xb_main_fn, void *);
typedef char *(*xb_prefpath_fn)(const char *, const char *);
typedef void (*xb_free_fn)(void *);
typedef int (*xb_sethint_fn)(const char *, const char *);
static HMODULE sdl3;

extern int openbor_main(int argc, char *argv[]);   /* OpenBOR's main (sdlport.c): a function really named main would make the C runtime skip WinMain */

/* "<...>\Local\Packages\<name>_<publisher id>\LocalState\" -> "<...>\Local\Publishers\<publisher id>\games\openbor" */
char xbox_openbor_home[MAX_PATH * 2];

/* Diagnostics stay in player builds: xbox_log.txt in the app's LocalState (OpenBOR/OpenBOR) */
static char log_path[MAX_PATH * 2];

void xbox_log(const char *fmt, ...)
{
    FILE *f = log_path[0] ? fopen(log_path, "ab") : NULL;
    if (f) {
        va_list ap;
        va_start(ap, fmt);
        vfprintf(f, fmt, ap);
        va_end(ap);
        fputc(13, f);
        fputc(10, f);
        fclose(f);
    }
}

const char *xbox_log_dir(void)
{
    static char dir[MAX_PATH * 2];
    char *cut;
    snprintf(dir, sizeof(dir), "%s", log_path);
    cut = strrchr(dir, 0x5C);
    if (cut) {
        *cut = 0;
    }
    return dir;
}

static void find_home(void)
{
    xb_prefpath_fn get_pref = (xb_prefpath_fn)(void *)GetProcAddress(sdl3, "SDL_GetPrefPath");
    xb_free_fn sdl_free = (xb_free_fn)(void *)GetProcAddress(sdl3, "SDL_free");
    char *pref = get_pref ? get_pref("OpenBOR", "OpenBOR") : NULL;   /* inside LocalState */
    xbox_openbor_home[0] = 0;
    if (!pref) {
        return;
    }
    snprintf(log_path, sizeof(log_path), "%sxbox_log.txt", pref);
    DeleteFileA(log_path);
    char *pk = strstr(pref, "\\Packages\\");
    char *ls = pk ? strstr(pk, "\\LocalState") : NULL;
    char *us = pk ? strrchr(pk + 10, '_') : NULL;
    if (pk && ls && us && us < ls) {
        char pub[64] = {0};
        size_t n = (size_t)(ls - us - 1);
        if (n < sizeof(pub)) {
            memcpy(pub, us + 1, n);
            snprintf(xbox_openbor_home, sizeof(xbox_openbor_home), "%.*s\\Publishers\\%s\\games\\openbor",
                     (int)(pk - pref), pref, pub);
        }
    }
    if (!xbox_openbor_home[0]) {   /* not packaged (PC test): next to LocalState's own folder */
        snprintf(xbox_openbor_home, sizeof(xbox_openbor_home), "%s", pref);
        size_t l = strlen(xbox_openbor_home);
        if (l && xbox_openbor_home[l - 1] == '\\') {
            xbox_openbor_home[l - 1] = 0;
        }
    }
    if (sdl_free) {
        sdl_free(pref);
    }
    char sub[MAX_PATH * 3];
    snprintf(sub, sizeof(sub), "%s\\..", xbox_openbor_home);
    CreateDirectoryA(sub, NULL);
    CreateDirectoryA(xbox_openbor_home, NULL);
}

static FILETIME launch_time;
static int launch_stamp(FILETIME *out);
static char pak_arg[MAX_PATH * 4];

static int run_openbor(int argc, char **argv)
{
    static char *args[3] = { (char *)"OpenBOR.exe", NULL, NULL };
    int n = 1;
    find_home();
    xbox_log("home: %s", xbox_openbor_home);
    char file[MAX_PATH * 3];
    snprintf(file, sizeof(file), "%s\\launch.txt", xbox_openbor_home);
    FILE *f = fopen(file, "rb");
    if (f) {
        char name[MAX_PATH] = {0};
        size_t got = fread(name, 1, sizeof(name) - 1, f);
        fclose(f);
        name[got] = 0;
        name[strcspn(name, "\r\n")] = 0;
        if (name[0]) {
            if (strchr(name, ':')) {   /* a whole path, as Game Vault writes it */
                snprintf(pak_arg, sizeof(pak_arg), "%s", name);
            } else {
                snprintf(pak_arg, sizeof(pak_arg), "%s\\Paks\\%s", xbox_openbor_home, name);
            }
            args[n++] = pak_arg;
        }
    }
    (void)argc;
    (void)argv;
    launch_stamp(&launch_time);
    xbox_log("start: %s", n > 1 ? args[1] : "(no game: menu)");
    return openbor_main(n, args);
}

static int launch_stamp(FILETIME *out)
{
    char file[MAX_PATH * 3];
    WIN32_FILE_ATTRIBUTE_DATA data;
    snprintf(file, sizeof(file), "%s\\launch.txt", xbox_openbor_home);
    if (!GetFileAttributesExA(file, GetFileExInfoStandard, &data)) {
        return 0;
    }
    *out = data.ftLastWriteTime;
    return 1;
}

/* Game Vault chose another game while this one waited in the background */
int xbox_launch_changed(void)
{
    FILETIME now;
    return launch_stamp(&now) && CompareFileTime(&now, &launch_time) != 0;
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show)
{
    (void)inst; (void)prev; (void)cmdline; (void)show;
    sdl3 = LoadLibraryA("SDL3.dll");
    xb_runapp_fn run_app = sdl3 ? (xb_runapp_fn)(void *)GetProcAddress(sdl3, "SDL_RunApp") : NULL;
    xb_sethint_fn set_hint = sdl3 ? (xb_sethint_fn)(void *)GetProcAddress(sdl3, "SDL_SetHint") : NULL;
    if (!run_app) {
        return 1;
    }
    /* Windows.Gaming.Input is the only controller API that works in Xbox apps */
    if (set_hint) {
        set_hint("SDL_JOYSTICK_WGI", "1");
        /* Direct3D 11 in SDL for UWP draws at full pixels into a swap chain of DIP size, no scaling: a quarter of the picture */
        set_hint("SDL_RENDER_DRIVER", "direct3d12");
    }
    return run_app(0, NULL, run_openbor, NULL);
}
