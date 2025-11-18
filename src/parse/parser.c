#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

static void	parser_state_init(t_parse_state *state, t_game *game)
{
    ft_bzero(state, sizeof(*state));
    state->game = game;
    state->fd = -1;
}

static void	parser_state_cleanup(t_parse_state *state)
{
    if (state->fd >= 0)
        close(state->fd);
    parser_free_map_lines(state->map_lines, state->map_count);
}

int	parse_cub_file(const char *path, t_game *game)
{
    t_parse_state   state;
    int             code;

    if (!path || !game)
        return (PARSE_ERR_ARG);
    parser_release_config(&game->cfg);
    ft_bzero(&game->cfg, sizeof(t_config));
    parser_state_init(&state, game);
    code = parser_process_file(&state, path);
    if (code == PARSE_OK)
        code = parser_normalize_map(&state);
    parser_state_cleanup(&state);
    if (code != PARSE_OK)
        parser_release_config(&game->cfg);
    return (code);
}

const char   *parser_strerror(int code)
{
    static const char *g_errors[] = {
        [PARSE_ERR_ARG] = "Invalid parser arguments",
        [PARSE_ERR_EXTENSION] = "Map file must have a .cub extension",
        [PARSE_ERR_OPEN_FILE] = "Failed to open map file",
        [PARSE_ERR_ALLOC] = "Memory allocation failed during parsing",
        [PARSE_ERR_CONFIG_DUP] = "Configuration key defined more than once",
        [PARSE_ERR_CONFIG_MISSING] = "Required configuration entry is missing",
        [PARSE_ERR_TEXTURE] = "Texture entry is invalid or unreadable",
    [PARSE_ERR_COLOR] = "Color entry must be three integers between 0 and 255",
        [PARSE_ERR_MAP_MISSING] = "Map block is missing or empty",
        [PARSE_ERR_MAP_INVALID_CHAR] = "Map contains invalid characters",
        [PARSE_ERR_MAP_GAP] = "Empty line inside map block",
        [PARSE_ERR_PLAYER_COUNT] = "Map must contain exactly one player",
        [PARSE_ERR_MAP_OPEN] = "Map is not enclosed by walls"
    };

    if (code == PARSE_OK)
        return ("No error");
    if (code < PARSE_ERR_ARG || code > PARSE_ERR_MAP_OPEN)
        return ("Unknown parser error");
    if (!g_errors[code])
        return ("Unknown parser error");
    return (g_errors[code]);
}
