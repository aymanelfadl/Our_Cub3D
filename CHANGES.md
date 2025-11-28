# Changes made during parser hang debugging and fixes

Summary
- Fixed an infinite loop in `find_map_width` which caused the program to hang during parsing.
- Corrected `find_map_start` so it skips leading whitespace and only considers lines starting with `1` or `0` as the map start.
- Added temporary diagnostics to `validate_map_chars` to help locate invalid characters (later removed).
- Removed all temporary debug output calls (`dprintf`, `write(2, ...)`, and `fprintf` debug messages) across the codebase after the issue was located.
- Cleaned up `validate_map_chars` to a stable implementation that simply returns `ERR_INVALID_CHAR` when encountering an invalid map character.

Files changed (high level)
- `src/parse/parse_map.c`
  - Fixed `find_map_width` off-by-one / missing increment.
  - Reworked `find_map_start` to skip whitespace and detect map start correctly.
  - Added then removed temporary diagnostics; final version contains a clean `validate_map_chars` implementation.
- `main.c`
  - Removed multiple temporary `dprintf`/`write` debug lines.
- `src/textures/loader.c`
  - Removed temporary `dprintf` debug prints; retained error prints.
- `raycasting/ray_casting.c`
  - Removed periodic render debug prints and fflush calls; preserved logic and MLX hook handling.

Why these changes
- The program was hanging in parser code while computing the map width. A missing increment in `find_map_width` caused an infinite loop. After fixing that, the parser incorrectly detected config lines as map rows; `find_map_start` was updated to avoid treating texture/color lines as map data.
- Temporary logging was added to quickly identify the failing functions and then removed to keep the code clean once the bugs were fixed.

How to verify
1. Build: `make -j2`
2. Run: `./cub3D maps/good/dungeon.cub`
   - The program should start, load textures and create the window without hanging.
3. Test parser-only: use the provided tests in `tests/` or run `./parser_tester` if available.

Notes
- I kept functional error messages (e.g., `fprintf(stderr, "Error\n...")`) for runtime errors; only temporary debug traces were removed.
- If you want the precise diagnostics re-enabled temporarily for other maps, I can re-add a compact diagnostic mode behind a compile flag.

Committed as: "Remove debug traces and fix parser map-start detection and validate_map_chars cleanup"
