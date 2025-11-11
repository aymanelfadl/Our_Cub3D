#include "cub3D.h"


void draw_background(t_game *game, int ceil_color, int floor_color)
{
    int y;
    int x;
    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        x = 0;
        while (x < WINDOW_WIDTH)
        {
            if (y < WINDOW_HEIGHT / 2)
                my_mlx_pixel_put(&game->frame, x, y, ceil_color);
            else
                my_mlx_pixel_put(&game->frame, x, y, floor_color);
            x++;
        }
        y++;
    }
}

void draw_vertical_line(t_game *game, int x)
{
    int lineHeight;
    float wall_x;
    t_img texture;

    float dist;
    if (!game->cfg.player.ray.hit.side) // vertical wall
    {
        dist = game->cfg.player.ray.distance_x - game->cfg.player.ray.next_cell_x;
        if (game->cfg.player.ray.ray_x > 0)
            texture = get_proper_texture(game->cfg.textures, WE);
        else
            texture = get_proper_texture(game->cfg.textures, EA);
    }
    else // horizontal 
    {
        dist = game->cfg.player.ray.distance_y - game->cfg.player.ray.next_cell_y;
        if (game->cfg.player.ray.ray_y > 0)
            texture = get_proper_texture(game->cfg.textures, NO);
        else
            texture = get_proper_texture(game->cfg.textures, SO);
    }

    lineHeight = (int)(WINDOW_HEIGHT / dist);

    // Wall hit position for texture mapping
    if (!game->cfg.player.ray.hit.side)
        wall_x = game->cfg.player.pos_y + dist * game->cfg.player.ray.ray_y;
    else
        wall_x = game->cfg.player.pos_x + dist * game->cfg.player.ray.ray_x;

    wall_x -= floor(wall_x);
    int tex_x = (int)(wall_x * (float)texture.width);

    int drawStart = -lineHeight / 2 + WINDOW_HEIGHT / 2;
    if (drawStart < 0)
        drawStart = 0;
    int drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
    if (drawEnd >= WINDOW_HEIGHT)
        drawEnd = WINDOW_HEIGHT - 1;

    float step = (float)texture.height / lineHeight;
    float texPos = (drawStart - WINDOW_HEIGHT / 2 + lineHeight / 2) * step;
    

    while (drawStart <= drawEnd)
    {
        int tex_y = (int)texPos;
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= texture.height)
            tex_y = texture.height - 1;

        texPos += step;

        size_t off = (size_t)tex_y * (size_t)texture.line_len + (size_t)tex_x * (texture.bpp / 8);
        unsigned int color = *(unsigned int *)(texture.addr + off);
        my_mlx_pixel_put(&game->frame, x, drawStart, color);
        drawStart++;
    }
}