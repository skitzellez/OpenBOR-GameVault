@echo off
rem Copyright (c) 2026 Skitzelles - part of Game Vault, https://github.com/skitzellez/xbox-game-vault
rem Builds xbox\dist\OpenBORForGameVault_x64.appx (signed with Game Vault's certificate, same publisher).
setlocal
set SDK=C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64
set R=%~dp0..\..
set GV=%R%\GameVault
cd /d "%~dp0"
if exist stage rmdir /s /q stage
mkdir stage\Assets
copy /y pkg\AppxManifest.xml stage\ >nul
copy /y pkg\Assets\*.png stage\Assets\ >nul
copy /y ..\build-xbox\OpenBOR.exe stage\ >nul || exit /b 1
copy /y "%R%\sdl2-compat\build-uwp\SDL2.dll" stage\ >nul || exit /b 1
copy /y "%R%\sdl3-uwp\build-uwp\RelWithDebInfo\SDL3.dll" stage\ >nul || exit /b 1
xcopy /e /i /q /y licenses stage\Assets\Licenses >nul || exit /b 1
for /d %%d in ("C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\*") do set "EB=%%d\bin\Hostx64\x64"
"%EB%\editbin.exe" /nologo /APPCONTAINER stage\OpenBOR.exe || exit /b 1
if not exist dist mkdir dist
if exist dist\OpenBORForGameVault_x64.appx del dist\OpenBORForGameVault_x64.appx
"%SDK%\makeappx.exe" pack /o /d stage /p dist\OpenBORForGameVault_x64.appx >nul || exit /b 1
if not defined PFX_PASSWORD for /f "usebackq delims=" %%p in (`powershell -NoProfile -ExecutionPolicy Bypass -File "%GV%\pfx_password.ps1"`) do set "PFX_PASSWORD=%%p"
if not defined PFX_PASSWORD (echo the certificate password could not be read & exit /b 1)
"%SDK%\signtool.exe" sign /fd SHA256 /a /f "%GV%\cert\GameVault.pfx" /p %PFX_PASSWORD% dist\OpenBORForGameVault_x64.appx >nul || exit /b 1
rmdir /s /q stage
echo PACKAGE OK
