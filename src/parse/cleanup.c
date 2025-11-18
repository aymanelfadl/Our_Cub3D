#include "parser_internal.h"

static void free_map_grid(t_map *map)
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
}

static void free_textures(t_config *cfg)
{
    int i;

    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (cfg->textures[i].path)
        {
            free(cfg->textures[i].path);
            cfg->textures[i].path = NULL;
        }
        i++;
    }
    ft_bzero(cfg->textures, sizeof(cfg->textures));
}

void parser_release_config(t_config *cfg)
{
    if (!cfg)
        return;
    free_textures(cfg);
    free_map_grid(&cfg->map);
    ft_bzero(&cfg->floor_color, sizeof(t_color));
    ft_bzero(&cfg->ceiling_color, sizeof(t_color));
    ft_bzero(&cfg->player, sizeof(t_player));
    cfg->have_floor = 0;
    cfg->have_ceiling = 0;
    cfg->have_player = 0;
}

void parser_free_map_lines(char **lines, int count)
{
    int i;

    if (!lines)
        return;
    i = 0;
    while (i < count)
    {
        free(lines[i]);
        i++;
    }
    free(lines);
}
