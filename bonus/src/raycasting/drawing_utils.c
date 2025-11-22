#include "cub3D.h"

int get_wall_hit(t_game *game, t_img texture)
{
    float wall_x;
    int tex_x;

    if (!game->cfg.player.ray.hit.side)
        wall_x = game->cfg.player.pos_y + game->cfg.player.ray.distance_x * game->cfg.player.ray.ray_y;
    else
        wall_x = game->cfg.player.pos_x + game->cfg.player.ray.distance_y * game->cfg.player.ray.ray_x;
    
    wall_x -= floor(wall_x);
    
    tex_x = (int)(wall_x * texture.width);
    
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= texture.width)
        tex_x = texture.width - 1;
    
    return (tex_x);
}


t_img get_texture(t_game *game)
{
    int hit_side;

    hit_side = game->cfg.player.ray.hit.side;
    if (!hit_side)
    {
        if (game->cfg.player.ray.ray_x > 0)
            return  get_proper_texture(game->cfg.textures, WE);
        else
            return get_proper_texture(game->cfg.textures, EA);
    }
    else // ver
    {
        if (game->cfg.player.ray.ray_y > 0)
            return get_proper_texture(game->cfg.textures, NO);
        else
            return get_proper_texture(game->cfg.textures, SO);
    }
}
