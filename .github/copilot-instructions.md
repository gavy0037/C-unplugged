## Quick orientation for AI coding agents

This repo is a small C command-line music library manager. The goal of this file is to capture the essential, concrete patterns an AI should know to be productive editing or extending the code.

- Project layout (important files)
  - `src/` — C sources: `main.c`, `library.c`, `album.c`, `song.c`, `playlist.c`.
  - `include/` — headers that define structs and APIs: `song.h`, `album.h`, `playlist.h`, `library.h`.
  - `data/` — runtime text files used for persistence: `songs.txt`, `albums.txt`, `playlist.txt`.
  - `bin/mp` — program built by `make` (target `run` builds `bin/mp`).

- Build & run
  - Build: run `make` from project root. The Makefile compiles with `gcc -Wall -Wextra -I./include` and writes object files to `build/` and binary to `bin/mp`.
  - Run: `./bin/mp` (Windows PowerShell: `.in\mp`). Non-interactive testing can be done by piping choices, e.g. `echo 11 | .\bin\mp` to save & exit.

- Data formats (exact, machine-readable)
  - `data/songs.txt`: lines are `id,name,artist,duration` (no spaces in tokens). Example: `1,Blinding_Lights,The_Weeknd,3:20`.
  - `data/albums.txt`: lines: `album_id,album_name,songId1|songId2|...`
  - `data/playlist.txt`: lines: `playlist_name,songId1|songId2|...`

- Runtime data structures & ownership patterns
  - Song: singly-linked list (`Song` in `include/song.h`). `load_songs` constructs the library's central song list (`lib->songs`).
  - Album: `Album` nodes hold a `songlist` which, when loaded from file, is built by creating *copies* of library songs (see `src/album.c::load_albums`). Albums therefore own their `Song` nodes independently of the library list.
  - Playlist: `Playlist` has a circular doubly-linked list of `Playlistnode` holding `Song *song`. Playlists typically reference the library `Song` nodes (not copies) when loaded or when user adds songs (see `src/playlist.c`). This mixed ownership (copies in albums, shared pointers in playlists) is an important invariant to preserve when modifying persistence or removal logic.

- Important functions to inspect when changing behavior
  - Loading/saving: `src/song.c::load_songs` / `save_songs`, `src/album.c::load_albums` / `save_albums`, `src/playlist.c::load_playlists` / `save_playlist`.
  - CLI and program flow: `src/main.c` (menu handlers call library/playlist functions). When adding menu options, keep input parsing consistent with existing patterns (reads expect single-token names, no spaces).
  - Memory/list helpers: `src/library.c`, `src/album.c`, `src/playlist.c` contain add/remove/print/free functions. Be careful with circular playlist lists (edge cases: size 0/1).

- Common pitfalls observed in the repo (learned from history)
  - Uninitialized pointer dereference: e.g. `create_playlist` previously dereferenced `curr` before initialization. When allocating structs, always initialize all pointer fields before use.
  - Ownership mismatch: albums contain copies of songs but playlists reference library song pointers. When removing songs from the library be mindful of playlists that may still point to them.
  - Text parsing: data files assume tokens without spaces; the code predominantly uses `strtok` and `sscanf("%[^,],%s")` patterns. Maintain these formats unless changing all readers/writers consistently.

- Testing & debugging tips
  - Quick run: `make && echo 10 | .\bin\mp` to print playlists non-interactively.
  - To debug memory issues compile with `-g -O0 -fsanitize=address,undefined` (adjust Makefile) and run under the sanitizer on supported platforms (Linux/macOS). On Windows, use the debugger or port to WSL for sanitizer runs.

- How to extend safely (practical checklist for code edits)
  1. Locate the module owning the concern: songs -> `src/song.c`, albums -> `src/album.c`, playlists -> `src/playlist.c`.
  2. Preserve file formats or update all readers/writers together.
  3. Initialize newly allocated structs immediately (set pointer members to NULL, sizes to 0).
  4. When changing persistence, include a quick script or run sequence to verify round-trip load/save using `data/*.txt`.

If anything here is unclear or you want the agent to add automated tests or a small test harness that runs several menu choices non-interactively, tell me which flows to script and I will add them.
