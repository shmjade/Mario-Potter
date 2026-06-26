# Mario Potter

A Harry Potter-themed remake of *Mario Bros*, written in C with [raylib](https://www.raylib.com/).
Originally developed as a university project (see `Relatório MarioPotter.pdf` for the full report, in Portuguese).

Play as Harry Potter, jump across platforms, collect coins, defeat Draco and Voldemort, and earn points for your Hogwarts house.

## Features

- **Menu** with multiple options: new game, continue, load custom map, ranking, sorting hat, about
- **Sorting Hat** — assigns the player to Gryffindor, Hufflepuff, Ravenclaw, or Slytherin
- **4 built-in phases** (`Phases/fase1.txt` … `fase4.txt`), defined as ASCII maps
- **Custom maps** — load any phase file with the same format
- **Save & load** — press `A` mid-game to save state; saved games are stored as files (e.g. `jade`, `vivi`)
- **Rankings** persisted in `highscores.bin`, tracked per player and per house
- **Sound effects and music** for spells, jumps, coin pickups, theme song, etc.

## Controls

- **← / →** — move
- **Space / ↑** — jump
- **Power button** — cast spell (limited uses per game, see `MAX_POWER`)
- **A** — save current game

## Requirements

- A C compiler (`gcc` or `clang`)
- [raylib](https://www.raylib.com/) 3.x or later
- Optional: [Code::Blocks](https://www.codeblocks.org/) IDE (the project ships with a `.cbp` file)

> **Important**: the game loads assets with relative paths (`Images/...`, `Audio/...`, `Fonts/...`, `Phases/...`). Always run the binary from the **repository root**, not from `bin/Debug/`.

## Installation & run

Clone the repo first:

```bash
git clone https://github.com/<your-user>/Mario-Potter.git
cd Mario-Potter
```

Then follow the instructions for your platform.

### macOS

1. Install [Homebrew](https://brew.sh) if you don't have it.
2. Install raylib:
   ```bash
   brew install raylib
   ```
3. Build (the extra `-I` / `-L` flags are required on Apple Silicon, since `/opt/homebrew` is not on the default search path):
   ```bash
   clang main.c Sources/*.c -IHeaders \
       -I$(brew --prefix raylib)/include \
       -L$(brew --prefix raylib)/lib \
       -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit \
       -framework CoreVideo -framework CoreAudio \
       -o MarioPotter
   ```
   Tested on macOS 15 (Sequoia) with raylib 5.5 and Apple clang.
4. Run from the repo root:
   ```bash
   ./MarioPotter
   ```

### Linux

#### Debian / Ubuntu

1. Install build tools and raylib's dependencies:
   ```bash
   sudo apt update
   sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev \
       libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
   ```
2. Install raylib. On recent Ubuntu/Debian:
   ```bash
   sudo apt install libraylib-dev
   ```
   If that package isn't available, build raylib from source:
   ```bash
   git clone https://github.com/raysan5/raylib.git
   cd raylib/src && make PLATFORM=PLATFORM_DESKTOP
   sudo make install
   cd ../..
   ```
3. Build:
   ```bash
   gcc main.c Sources/*.c -IHeaders \
       -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
       -o MarioPotter
   ```
4. Run from the repo root:
   ```bash
   ./MarioPotter
   ```

#### Arch Linux

```bash
sudo pacman -S raylib base-devel
gcc main.c Sources/*.c -IHeaders -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o MarioPotter
./MarioPotter
```

#### Fedora

```bash
sudo dnf install raylib raylib-devel gcc make
gcc main.c Sources/*.c -IHeaders -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o MarioPotter
./MarioPotter
```

### Windows

The easiest path on Windows is **MSYS2 + MinGW-w64**.

1. Install [MSYS2](https://www.msys2.org/) and open the **MINGW64** shell.
2. Install the toolchain and raylib:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib git
   ```
3. From the repo root, build:
   ```bash
   gcc main.c Sources/*.c -IHeaders -lraylib -lopengl32 -lgdi32 -lwinmm -o MarioPotter.exe
   ```
4. Run:
   ```bash
   ./MarioPotter.exe
   ```

Alternatively, you can open `MarioPotter1.0.cbp` in **Code::Blocks for Windows** (the version bundled with MinGW) and just press **Build & Run**. Make sure raylib's headers and `libraylib.a` are on Code::Blocks' search paths.

### Using Code::Blocks (any platform)

1. Install [Code::Blocks](https://www.codeblocks.org/downloads/) with a GCC toolchain.
2. Install raylib for your platform (see above).
3. Open `MarioPotter1.0.cbp`.
4. In **Project → Build options**, add `raylib` and any platform-specific libraries (see the commands above) to the **Linker settings**.
5. Build the `Debug` or `Release` target. The output goes to `bin/Debug/` or `bin/Release/`.
6. Copy the binary to the repo root (or set the working directory to the repo root in **Project → Properties → Build targets**), then run it.

## Project layout

```
main.c                    — entry point and main menu loop
Headers/                  — header files (structs, definitions, function declarations)
Sources/                  — implementation (.c) files
Phases/                   — phase map files (ASCII grids, 29 × 120 chars)
Images/                   — sprites and textures
Audio/                    — music and sound effects
Fonts/                    — Harry Potter custom font
doxygen/                  — generated API documentation
Relatório MarioPotter.pdf — original project report (Portuguese)
highscores.bin            — persisted rankings
```

### Map file format

Each phase file is a 29-row × 120-column grid. Characters represent map elements (platforms, coins, enemies, pipes, etc.), with one row corresponding to 25 vertical pixels and one column to 10 horizontal pixels.

## Code structure

The game loop in `main.c` calls into the menu, which dispatches to one of:

| Option | Function | Module |
|---|---|---|
| New game | `play_new_game` | `game_functions.c` |
| Continue | `continue_game` | `game_functions.c` |
| Load map | `load_map` | `load_media.c` |
| Ranking | `ranking_screen` | `game_functions.c` |
| Sorting hat | `sorting_hat` | `draw_functions.c` |
| About | `about_us` | `menu_functions.c` |

Game state lives in the `PHASE`, `HERO`, `ENEMY`, and `GAME` structs defined in `Headers/structs.h`.

## Notes

- Source comments and the original report are in Brazilian Portuguese.
- The project was built for 1200 × 700 resolution at 60 FPS.
