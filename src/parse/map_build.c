#include "parser_internal.h"

static void destroy_map_grid(t_map *map)
{
    int i;

    if (!map->grid)
        return;
    i = 0;
    while (i < map->height)
    {
        free(map->grid[i]);
        i++;
    }
    free(map->grid);
    map->grid = NULL;
    map->height = 0;
    map->width = 0;
    map->sprite_count = 0;
}

static int create_normalized_grid(t_map *map, int height, int width)
{
    int i;

    map->grid = ft_calloc(height, sizeof(char *));
    if (!map->grid)
        return (0);
    i = 0;
    while (i < height)
    {
        map->grid[i] = ft_calloc(width + 1, sizeof(char));
        if (!map->grid[i])
        {
            while (i > 0)
            {
                i--;
                free(map->grid[i]);
            }
            free(map->grid);
            map->grid = NULL;
            return (0);
        }
        ft_memset(map->grid[i], ' ', width);
        map->grid[i][width] = '\0';
        i++;
    }
    map->height = height;
    map->width = width;
    return (1);
}

static int compute_width(t_parse_state *state)
{
    int width;
    int i;
    int len;

    width = 0;
    i = 0;
    while (i < state->map_count)
    {
        len = (int)ft_strlen(state->map_lines[i]);
        if (len > width)
            width = len;
        i++;
    }
    return (width);
}

static int fill_row(t_parse_state *state, int row, int *player_count)
{
    t_config    *cfg;
    char        *src;
    int         col;
    int         len;
    char        c;

    cfg = &state->game->cfg;
    src = state->map_lines[row];
    if (!src)
        return (PARSE_ERR_MAP_INVALID_CHAR);
    len = (int)ft_strlen(src);
    col = 0;
    while (col < len)
    {
        c = src[col];
        cfg->map.grid[row][col] = c;
        if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
        {
            parser_set_player(cfg, row, col, c);
            cfg->map.grid[row][col] = '0';
            (*player_count)++;
        }
        else if (c == '2')
            cfg->map.sprite_count += 1;
        col++;
    }
    return (PARSE_OK);
}

static int populate_rows(t_parse_state *state, int *player_count)
{
    int i;
    int code;

    *player_count = 0;
    i = 0;
    while (i < state->map_count)
    {
        code = fill_row(state, i, player_count);
        if (code != PARSE_OK)
            return (code);
        i++;
    }
    return (PARSE_OK);
}

int parser_normalize_map(t_parse_state *state)
{
    int height;
    int width;
    int player_count;
    int code;

    height = state->map_count;
    if (height <= 0)
        return (PARSE_ERR_MAP_MISSING);
    width = compute_width(state);
    if (!create_normalized_grid(&state->game->cfg.map, height, width))
        return (PARSE_ERR_ALLOC);
    state->game->cfg.map.sprite_count = 0;
    code = populate_rows(state, &player_count);
    if (code != PARSE_OK)
    {
        destroy_map_grid(&state->game->cfg.map);
        return (code);
    }
    code = parser_finalize_player(&state->game->cfg, player_count);
    if (code != PARSE_OK)
    {
        destroy_map_grid(&state->game->cfg.map);
        return (code);
    }
    if (state->game->cfg.map.sprite_count > 0
        && !state->game->cfg.have_sprite)
    {
        destroy_map_grid(&state->game->cfg.map);
        return (PARSE_ERR_TEXTURE);
    }
    code = parser_validate_enclosure(&state->game->cfg.map);
    if (code != PARSE_OK)
    {
        destroy_map_grid(&state->game->cfg.map);
        return (code);
    }
    parser_free_map_lines(state->map_lines, state->map_count);
    state->map_lines = NULL;
    state->map_count = 0;
    state->map_cap = 0;
    return (PARSE_OK);
}
