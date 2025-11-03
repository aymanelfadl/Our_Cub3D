# Cub3D Refactor Overview

## Architecture
- Modular parser lives under `src/parse/` and is surfaced through `includes/parser.h`; `main.c` now drives `parse_cub_file` and frees resources via `parser_release_config`.

## Parsing & Validation
- Flood-fill enclosure (`floodfill.c`), map normalization and sprite counting (`map_build.c`), and directive checks in the validation modules now emit structured error codes instead of exiting abruptly.

## State & Data
- `t_config` and `t_map` now track resolution, sprite metadata, and readiness flags inside `includes/cub3D.h`; the legacy parser remains in `backup_old_parser/` for reference.

## Build & Tooling
- `Makefile` compiles the new parser sources, adds a `parser_tester` target with fixtures in `tests/`, links against the repo’s MiniLibX build (`minilibx-linux`), and disables PIE for compatibility.

## Graphics Integration
- `includes/cub3D.h` pulls in the MiniLibX header via a repo-relative path, keeping parser and renderer builds consistent without depending on a system-wide install.

## Testing & Runtime
- Regression tests and the Valgrind leak check cover both happy and error paths, and `./cub3D maps/good/cheese_maze.cub` now renders a frame pending keyboard event hooks.
