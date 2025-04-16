# multisweeper

## Setup

I'm assuming your using Windows

## Backend (C++)

### Running

1. Install MSYS2 (for building with g++ cause my homies hating on msvc)
2. Update packages and install mingw compiler and pkg-config with `pacman -Syu` and `pacman -S mingw-w64-x86_64-gcc` and optionally `pacman -S mingw-w64-x86_64-pkgconf mingw-w64-x86_64-zlib mingw-w64-x86_64-libuv`
3. Add mingw to you path `C:\msys64\mingw64\bin` in the edit system variable environment windows thingy
4. Install Meson and Ninja: `pip install meson` and `pip install ninja`.
5. Navigate to `server/`.
6. Run `meson setup builddir && meson compile -C builddir`.
7. Run the server: `./builddir/multisweeper-server.exe`.

### Developer

1. Install clang-tidy and clang-format with MSYS2 (okay maybe built into c++ vscode extension already)
2. Documention and testing later x3

## Frontend (React)

### Running

1. Navigate to `client/`.
2. Install dependencies: `npm install`.
3. Start the dev server: `npm run dev`.

## Structure

- `backend/`: C++ server with Meson build system.
- `frontend/`: React app with TypeScript, TanStack Query, and MUI.
