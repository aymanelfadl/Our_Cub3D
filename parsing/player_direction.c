#include "cub3D.h"

void set_player_dir(t_game *game, char c)
{
    if (c == 'N')
    {
        game->cfg.player.dir_x = 0;
        game->cfg.player.dir_y = -1;
    }
    else if (c == 'S')
    {
        game->cfg.player.dir_x = 0;
        game->cfg.player.dir_y = 1;
    }
    else if (c == 'E')
    {
        game->cfg.player.dir_x = 1;
        game->cfg.player.dir_y = 0;
    }
    else
    {
        game->cfg.player.dir_x = -1;
        game->cfg.player.dir_y = 0;
    }
}

void set_player_plane(t_game *game)
{
    double fov_rad;
    double plane_len;

    fov_rad  = FOV_PLANE * M_PI / 180.0;
    plane_len = tan(fov_rad / 2.0);
    
    game->cfg.player.plane_x = -game->cfg.player.dir_y * plane_len;
    game->cfg.player.plane_y =  game->cfg.player.dir_x * plane_len;
}

t_direction get_direction(t_game *game, char c)
{
    set_player_dir(game, c);
    set_player_plane(game);

    if (c == 'N') return NO;
    if (c == 'S') return SO;
    if (c == 'E') return EA;
    return WE;
}
