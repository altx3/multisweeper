# multisweeper

## Setup

I'm assuming your using Windows

## Backend (C++)

### Running

1. Install Meson and Ninja: `pip install meson` and `pip install ninja`.
2. Navigate to `server/`.
3. Run `meson setup builddir && meson compile -C builddir`.
4. Run the server: `./builddir/multisweeper-server.exe`.

### Developing

1. Install clang-tidy and clang-format with MSYS2 (okay maybe built into c++ vscode extension already)
2. Documention and testing later x3
