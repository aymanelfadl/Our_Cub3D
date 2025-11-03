#!/bin/sh

ROOT_DIR=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
if [ -z "$ROOT_DIR" ]; then
    echo "Unable to locate project root" >&2
    exit 1
fi
cd "$ROOT_DIR" || exit 1

make parser_tester >/dev/null || exit 1

STATUS=0

run_test() {
    file=$1
    expected=$2
    label=$3
    ./parser_tester "$file" >/dev/null 2>&1
    result=$?
    if [ "$expected" -eq 0 ]; then
        if [ "$result" -eq 0 ]; then
            printf "[OK] %s\n" "$label"
        else
            printf "[FAIL] %s (expected success, got %d)\n" "$label" "$result"
            STATUS=1
        fi
    else
        if [ "$result" -ne 0 ]; then
            printf "[OK] %s\n" "$label"
        else
            printf "[FAIL] %s (expected failure)\n" "$label"
            STATUS=1
        fi
    fi
}

run_test tests/parser/valid.cub 0 "valid.cub"
run_test tests/parser/missing_player.cub 1 "missing_player.cub"
run_test tests/parser/multiple_players.cub 1 "multiple_players.cub"
run_test tests/parser/open_map.cub 1 "open_map.cub"
run_test tests/parser/ragged_map.cub 0 "ragged_map.cub"
run_test tests/parser/invalid_char.cub 1 "invalid_char.cub"
run_test tests/parser/missing_texture.cub 1 "missing_texture.cub"

if command -v valgrind >/dev/null 2>&1; then
    valgrind --quiet --error-exitcode=1 ./parser_tester tests/parser/valid.cub >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        printf "[OK] valgrind leak check\n"
    else
        printf "[FAIL] valgrind leak check\n"
        STATUS=1
    fi
else
    printf "[WARN] valgrind not found, skipping leak check\n"
fi

exit $STATUS
