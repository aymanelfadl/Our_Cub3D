#include "cub3D.h"

int is_texture(char *id)
{
    return (!ft_strcmp(id, "NO") || !ft_strcmp(id, "SO") ||
            !ft_strcmp(id, "WE") || !ft_strcmp(id, "EA"));
}

void which_identifier(t_texture *tex, char *key)
{
    if (!ft_strcmp(key, "NO"))
        tex->id = NO;
    else if (!ft_strcmp(key, "SO"))
        tex->id = SO;
    else if (!ft_strcmp(key, "WE"))
        tex->id = WE;
    else if (!ft_strcmp(key, "EA"))
        tex->id = EA;
}

t_texture get_texture(char **map)
{
    t_texture tex;
    which_identifier(&tex, map[0]);
    if (check_extension(map[1], ".xpm"))
        tex.path = ft_strdup(map[1]);
    else
        return (print_err("Texture path"), tex);
    return tex;
}

int handle_texture(t_game *game, char **map)
{
    if (!is_texture(map[0]) || ft_split_size(map) != 2)
        return 0;

    t_texture tex = get_texture(map);
    int i;
    if (!ft_strcmp(map[0], "NO")) i = 0;
    else if (!ft_strcmp(map[0], "SO")) i = 1;
    else if (!ft_strcmp(map[0], "WE")) i = 2;
    else i = 3;

    if (game->cfg.textures[i].path)
        print_err("Duplicate direction");

    game->cfg.textures[i] = tex;
    return 1;
}

int game_config(t_game *game, char **map)
{
    if (!ft_strcmp(map[0], "F"))
        return handle_color(game, map, 1);
    else if (!ft_strcmp(map[0], "C"))
        return handle_color(game, map, 0);
    else if (handle_texture(game, map))
        return 1;
        
    return (print_err("Wrong identifier"), 0);
}
