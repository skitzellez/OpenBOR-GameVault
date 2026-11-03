# Copyright (c) 2026 Skitzelles - part of Game Vault, https://github.com/skitzellez/xbox-game-vault
# OpenBOR for Game Vault: Xbox (UWP Dev Mode) build, MSYS2 UCRT64 MinGW. SDL2 comes from sdl2-compat, which runs
# on our SDL3 for UWP (no OpenGL on the Xbox: SDL's own renderer draws). Entry point: engine/sdl/xbox_main.c.
set(CMAKE_RC_COMPILER "windres")
if(NOT CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH "$ENV{MINGW_PREFIX}")
endif()
set(XBOX_SDL2COMPAT "${PROJECT_SOURCE_DIR}/../sdl2-compat" CACHE PATH "sdl2-compat source+build")
add_definitions(-DXBOX_UWP=1)
target_sources(${PROJECT_NAME} PRIVATE engine/sdl/xbox_main.c engine/sdl/xbox_audio.c)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${XBOX_SDL2COMPAT}/include/SDL2
  ${XBOX_SDL2COMPAT}/include
  ${CMAKE_PREFIX_PATH}/include
  engine/source/adpcmlib
)
target_link_directories(${PROJECT_NAME} PRIVATE ${XBOX_SDL2COMPAT}/build-uwp ${CMAKE_PREFIX_PATH}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE -lole32 -mwindows)
set(COMMON_COMPILER_FLAGS "${COMMON_COMPILER_FLAGS} -Wno-address -Wno-enum-int-mismatch -Wno-stringop-truncation -Wno-maybe-uninitialized -Wno-deprecated-declarations -Wno-overflow -Wno-shift-count-overflow")
set(ARCH_SUFFIX "")
set(USE_SDL     ON)
set(USE_OPENGL  OFF)
set(USE_LOADGL  OFF)
set(USE_GFX     ON)
set(USE_VORBIS  ON)
set(USE_WEBM    ON)
set(USE_PTHREAD ON)
set(ENABLE_STATIC     ON)
set(ENABLE_STATIC_SDL OFF)
add_definitions(-DWIN -DELF -DPSAPI_VERSION=2)
