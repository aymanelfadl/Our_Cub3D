# Cub3D Refactor Overview

## Architecture
- Modular parser lives under `src/parse/` and is surfaced through `includes/parser.h`; `main.c` now drives `parse_cub_file` and frees resources via `parser_release_config`.

## Parsing & Validation
- Flood-fill enclosure (`floodfill.c`), map normalization and sprite counting (`map_build.c`), and directive checks in the validation modules now emit structured error codes instead of exiting abruptly.

## State & Data
- `t_config` and `t_map` now track resolution, sprite metadata, and readiness flags inside `includes/cub3D.h`; the legacy parser remains in `backup_old_parser/` for reference.

## Build & Tooling
- `Makefile` now compiles against the repo’s MiniLibX headers, triggers `make -C minilibx-linux` automatically, links with the local library, and disables PIE for compatibility.

## Graphics Integration
- `includes/cub3D.h` centralizes all MiniLibX constants, key codes, and player movement tunables, keeping renderer modules in sync.
- `raycasting/ray_casting.c` owns the runtime loop: it clears the frame each render, performs DDA ray casting, and pushes the frame buffer to the window.
- MLX hooks handle window close events and keyboard input using MiniLibX’s key codes; helper routines cover movement, rotation, collision checks, and clean shutdowns.

## Testing & Runtime
- Regression tests and the Valgrind leak check cover both happy and error paths. At runtime, `./cub3D maps/good/hanzo.cub` now opens a window, renders walls, and supports WASD rotation/translation (`Q`/`E` strafe, `Esc` exit).
