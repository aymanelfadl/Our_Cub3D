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

float get_texture_y(int *drawing_start, float text_step)
{
    float hidden_pixels;

    hidden_pixels = 0;
    if (*drawing_start < 0)
    {
        hidden_pixels = -(*drawing_start) * text_step;
        *drawing_start = 0;
    }
    return hidden_pixels;
}

t_img get_texture(t_game *game)
{
    if (game->cfg.player.ray.hit.is_hit == 1)
    {
        if (!game->cfg.player.ray.hit.side)
        {
            if (game->cfg.player.ray.ray_x > 0)
                return  get_proper_texture(game->cfg.textures, WE);
            else
                return get_proper_texture(game->cfg.textures, EA);
        }
        else
        {
            if (game->cfg.player.ray.ray_y > 0)
                return get_proper_texture(game->cfg.textures, NO);
            else
                return get_proper_texture(game->cfg.textures, SO);
        }
    }
    return game->cfg.door_texture.img;    
}

unsigned int get_texture_color(t_img texture, int tex_y, int tex_x)
{
    size_t offset;
    
    if (tex_y < 0)
        tex_y = 0;
    if (tex_y >= texture.height)
        tex_y = texture.height - 1;
    offset = (size_t)tex_y * (size_t)texture.line_len
            + (size_t)tex_x * (texture.bpp / 8);
    return (*(unsigned int *)(texture.addr + offset));
}