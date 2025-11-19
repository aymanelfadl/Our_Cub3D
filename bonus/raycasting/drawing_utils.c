#include "cub3D.h"

float get_wall_hit(t_game *game, float dist)
{
    float wall_x;
    // Wall hit position for texture mapping
    if (!game->cfg.player.ray.hit.side)
        wall_x = game->cfg.player.pos_y + dist * game->cfg.player.ray.ray_y;
    else
        wall_x = game->cfg.player.pos_x + dist * game->cfg.player.ray.ray_x;
    wall_x -= floor(wall_x);
    return (wall_x);
}

float get_dist(t_game *game, int hit_side)
{
    if (!hit_side)
        return (game->cfg.player.ray.distance_x - game->cfg.player.ray.next_cell_x);
    else
        return (game->cfg.player.ray.distance_y - game->cfg.player.ray.next_cell_y);
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

int get_drawing_start(int line_height)
{
    int drawStart;

    drawStart = - line_height / 2 + WINDOW_HEIGHT / 2;
    if (drawStart < 0)
        drawStart = 0;
    return (drawStart);
}

int get_drawing_end(int line_height)
{
    int drawEnd;

    drawEnd  = (line_height / 2) + (WINDOW_HEIGHT / 2);
    if (drawEnd >= WINDOW_HEIGHT)
        drawEnd = WINDOW_HEIGHT - 1;
    return  (drawEnd);
}
