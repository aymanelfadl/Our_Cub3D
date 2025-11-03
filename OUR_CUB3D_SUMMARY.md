# Parser Rewrite Summary

- Replaced the ad-hoc parsing flow with a modular parser (`src/parse/*.c`) exposed through `includes/parser.h`, wiring `main.c` to `parse_cub_file` and central `parser_release_config`.
- Built iterative flood-fill enclosure checking in `src/parse/floodfill.c`, map normalization/sprite counting in `src/parse/map_build.c`, and strict config validators in `src/parse/validation_*.c`, all returning error codes instead of exiting.
- Extended `t_config`/`t_map` with resolution, sprite, and state flags in `includes/cub3D.h`, while keeping old sources archived under `backup_old_parser/`.
- Updated `Makefile` to consume the new parser sources and added a `parser_tester` target plus regression assets (`tests/parser_main.c`, `.cub` fixtures, `tests/run_parser_tests.sh`); removed the global `<mlx.h>` include from the public header and limited it to `raycasting/ray_casting.c` so parser builds no longer depend on MLX headers.
- Added regression tests and a Valgrind-backed leak check covering success and failure cases for the parser.
