<p align="center">
  <img src="https://raw.githubusercontent.com/skitzellez/xbox-game-vault/main/GameVault/art/logo-banner.png" alt="Game Vault" width="720">
</p>

<h3 align="center">Game Vault · OpenBOR</h3>

<p align="center">
  <b>OpenBOR beat 'em ups on your Xbox, with the controller.</b><br>
  The OpenBOR engine for <a href="https://github.com/skitzellez/xbox-game-vault">Game Vault</a> on <b>Xbox Series X|S</b> in Dev Mode:
  pick a game in Game Vault and it plays here, then you are back in Game Vault.
</p>

<!-- gv-buttons:start -->
<p align="center">
  <a href="https://github.com/skitzellez/OpenBOR-GameVault/releases/latest"><img alt="DOWNLOAD OpenBOR" src="https://img.shields.io/badge/DOWNLOAD-OpenBOR-107C10?style=for-the-badge&logo=xbox&logoColor=white"></a>
  &nbsp;
  <a href="https://github.com/skitzellez/xbox-game-vault"><img alt="Get Game Vault" src="https://img.shields.io/badge/GET-Game%20Vault-107C10?style=for-the-badge&logo=xbox&logoColor=white"></a>
  &nbsp;
  <a href="https://discord.gg/RmhhWgguvw"><img alt="Join the Game Vault Discord" src="https://img.shields.io/badge/JOIN-Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white"></a>
  &nbsp;
  <a href="https://www.patreon.com/Skitzelles"><img alt="Support on Patreon" src="https://img.shields.io/badge/SUPPORT-Patreon-F96854?style=for-the-badge&logo=patreon&logoColor=white"></a>
</p>
<!-- gv-buttons:end -->

<p align="center">
  <img src="xbox/art/menu.png" alt="OpenBOR running on an Xbox Series console" width="720">
</p>

## 🎮 What it is

[OpenBOR](https://github.com/DCurrent/openbor) is a free, open-source engine for side-scrolling beat 'em ups. Each game is one `.pak` file, and fans have made thousands of them. This app is that engine, made for Xbox Series X|S in Dev Mode.

**No games are included.** Bring your own `.pak` files, and only play games you have the right to play.

## 1️⃣ 2️⃣ 3️⃣ How to play

| | |
|---|---|
| **1** | Install [Game Vault](https://github.com/skitzellez/xbox-game-vault). Its setup adds this app, or press RT in Game Vault to add it with your phone. |
| **2** | Put your `.pak` files anywhere on your games USB drive. |
| **3** | Open the **OpenBOR** shelf in Game Vault and press A. |

## 🕹️ Controls

| Controller | In the game |
|---|---|
| Left stick or D-pad | Move |
| X | Attack |
| A | Jump |
| B | Special |
| Y, LB, RB | Attacks 2, 3 and 4 |
| Menu | Start |
| Hold View + Menu for 2 seconds | Back to Game Vault |

Each game can use these buttons in its own way.

## 🧩 Which games work

This app uses OpenBOR Build 7533 (2024), which plays older games as well as many newer ones. A game made for a later engine may not start.

## 📜 Credits and licence

- The OpenBOR engine: © Roel van Mastbergen & Senile Team, the OpenBOR Team and contributors. BSD-3-Clause, see [LICENSE](LICENSE). The project's own readme is kept as [README.upstream.md](README.upstream.md).
- Xbox port: Skitzelles. Files in `xbox/`, `cmake/xbox.cmake`, `engine/sdl/xbox_main.c` and `engine/sdl/xbox_audio.c`.
- Other libraries used and their licences: [xbox/licenses](xbox/licenses).

This is a modified version of OpenBOR, not made or supported by the OpenBOR Team. Please report problems with this app to Game Vault, not to them.

<!-- gv-family:start -->
## 🎮 The Game Vault family

Game Vault is one launcher plus emulators built to work with it. **Install [Game Vault](https://github.com/skitzellez/xbox-game-vault) and it sets these up for you**; press **RT** in Game Vault to update them all.

**Apps**

| Project | What it is | Runs on |
|---|---|---|
| [Game Vault](https://github.com/skitzellez/xbox-game-vault) | The launcher: plug in a USB drive, pick a game, play. Sets up everything below by itself. | Xbox Series X&#124;S |
| [Game Vault for Android](https://github.com/skitzellez/game-vault-android) | The same game library for phones, tablets and Android TV (beta). | Android |
| [Game Vault for iPhone](https://github.com/skitzellez/game-vault-ios) | The Game Vault look on iPhone (early test). | iPhone |

**Emulators built for Game Vault**

| Project | What it is | Runs on |
|---|---|---|
| [xemu for Xbox Dev Mode](https://github.com/skitzellez/xemu-uwp) | Original Xbox | Xbox Series X&#124;S |
| [Xenia for Game Vault](https://github.com/skitzellez/Xenia-GameVault) | Xbox 360 | Xbox Series X&#124;S |
| [XBSX2 for Game Vault](https://github.com/skitzellez/XBSX2-GameVault) | PlayStation 2 | Xbox Series X&#124;S |
| [Dolphin for Game Vault](https://github.com/skitzellez/Dolphin-GameVault) | GameCube and Wii | Xbox Series X&#124;S |
| [Game Vault · Wii U (Cemu)](https://github.com/skitzellez/Cemu-GameVault) | Wii U | Xbox Series X&#124;S |
| [RetroArch for Game Vault](https://github.com/skitzellez/RetroArch-GameVault) | PlayStation, Dreamcast, arcade and classic systems | Xbox Series X&#124;S, Android |
| [Game Vault · Flash](https://github.com/skitzellez/Flash-GameVault) | Flash games | Xbox Series X&#124;S |
| [**Game Vault · OpenBOR**](https://github.com/skitzellez/OpenBOR-GameVault) 📍 *you are here* | OpenBOR beat 'em up games | Xbox Series X&#124;S |
| [Gambatte for Game Vault](https://github.com/skitzellez/Gambatte-GameVault) | Game Boy link cable in online rooms | Xbox Series X&#124;S, Android |
| [X1 BOX for Game Vault](https://github.com/skitzellez/x1box-gamevault) | Original Xbox | Android |

**Extras**

| Project | What it is | Runs on |
|---|---|---|
| [Free games](https://github.com/skitzellez/game-vault-free-games) | Freeware and shareware for the FREE GAMES tab | Game Vault |
| [Fallout 1in2 for Xbox](https://github.com/skitzellez/Fallout1in2-Xbox) | The original Fallout, made for the controller | Xbox Series X&#124;S |
| [Game Vault updates](https://github.com/skitzellez/game-vault-updates) | Update data Game Vault reads when it starts | Game Vault |

**💬 Help, news and bug reports:** the [Game Vault Discord](https://discord.gg/RmhhWgguvw) (or this repo's Issues).
Please don't take problems with these builds to the original emulator teams: they don't make or support them.
<!-- gv-family:end -->
