# multisweeper

## Setup

I'm assuming your using Windows

## Backend (C++)

### Requirements

1. A c and c++ compiler
2. Required
  - Ninja
  - zlib
  - libuv
  - python
  - meson
3. Optional
  - pkg-config
  - open-ssl
  - clang-tidy ~20.1.2
  - clang-format
  - afunix.h (if you on windows)

Documention and testing later x3

### Running
1. Navigate to `server/`.
2. Run `meson setup build -DuSockets:libuv=true && meson compile -C build`.
3. Run the server: `./build/multisweeper-server.exe`.

### My tooling I use for Windows
1. Install MSYS2 (for building with g++ cause my homies hating on msvc)
2. Update packages and install mingw gcc compiler with `pacman -Syu` and `pacman -S mingw-w64-x86_64-gcc` and optionally `pacman -S mingw-w64-x86_64-pkgconf mingw-w64-x86_64-zlib mingw-w64-x86_64-libuv` in the MSYS terminal
3. Add mingw to you path `C:\msys64\mingw64\bin` in the edit system variable environment windows thingy
4. uhhh idk where i got afunix.h from, think there is a pacman package or you could get install Visual Studio (not vscode!) or just the windows sdk thing
4. Install Meson and Ninja: `pip install meson` and `pip install ninja`.
5. Navigate to `server/`.
6. Just hit that sick task `Run server` in vscode

## Frontend (React)

### Requirements
1. Node.js
2. whatever is in the package.json

### Running

1. Navigate to `client/`.
2. Install dependencies: `npm install`.
3. Start the dev server: `npm run dev`.

## Structure

- `backend/`: C++ server with Meson build system.
- `frontend/`: React app with TypeScript, TanStack Query, and MUI.
