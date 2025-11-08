# Cub3D Refactor Overview

Status
------
- The changes from the `refactor/parser` branch were merged into `main` on 2025-11-08. The merge resolved conflicts by preferring the refactor branch content to bring `main` up to date with the refactor work. Please run the project's build/tests to validate the merged tree.


## Architecture
- Modular parser lives under `src/parse/` and is surfaced through `includes/parser.h`; `main.c` now drives `parse_cub_file` and frees resources via `parser_release_config`.
- Texture helpers live under `src/textures/` and expose `texture_load_all`, `texture_free_all`, and `tex_get_pixel` through `includes/cub3D.h`.

## Parsing & Validation
- Flood-fill enclosure (`floodfill.c`), map normalization and sprite counting (`map_build.c`), and directive checks in the validation modules now emit structured error codes instead of exiting abruptly.
- Legacy resolution directives (`R ...`) are no longer parsed; `parser_finalize_resolution` assigns defaults and will later clamp to the display caps, keeping parsing tolerant of older files.

## State & Data
- `t_config` and `t_map` now track resolution, sprite metadata, and readiness flags inside `includes/cub3D.h`; the legacy parser remains in `backup_old_parser/` for reference.
- Resolution fields are initialized during parser finalization, so runtime code can trust `cfg->resolution_width/height` without checking for the `R` token.

## Build & Tooling
- `Makefile` now compiles against the repo’s MiniLibX headers, triggers `make -C minilibx-linux` automatically, links with the local library, and disables PIE for compatibility.
- New parser sources such as `finalize_resolution.c` are part of the default build, so documentation stays in sync with parser behavior.

## Graphics Integration
- `includes/cub3D.h` centralizes MiniLibX handles, key codes, movement tunables, and texture descriptors (image metadata, load flags, accessors).
- `main.c` boots MLX early, loads all wall textures via `texture_load_all`, and aborts cleanly if any XPM fails.
- `raycasting/ray_casting.c` clears the frame buffer, performs DDA ray casting, samples the appropriate texture column, applies simple shading on perpendicular walls, and pushes the frame to the window.
- MLX hooks handle window close events and keyboard input (`WASD` + arrows for rotation, `Q/E` strafe); helper routines cover movement, collision checks, and shutdown.

## Testing & Runtime
- Regression tests and the Valgrind leak check cover both happy and error paths. At runtime, `./cub3D maps/good/hanzo.cub` or `maps/good/dungeon.cub` (without any `R` line) opens a window, renders textured walls, and supports WASD rotation/translation (`Q`/`E` strafe, `Esc` exit).
