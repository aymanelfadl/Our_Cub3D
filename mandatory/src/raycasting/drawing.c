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

unsigned int get_texture_color(t_img texture, int tex_y, int tex_x)
{
    size_t offset;
    
    if (tex_y < 0)
        tex_y = 0;
    if (tex_y >= texture.height)
        tex_y = texture.height - 1;
    offset = (size_t)tex_y * (size_t)texture.line_len + (size_t)tex_x * (texture.bpp / 8);
    return (*(unsigned int *)(texture.addr + offset));
}

void draw_vertical_line(t_game *game, int x, t_img texture)
{
    int line_height;
    int draw_start;
    int draw_end;
    int tex_x;
    float tex_y;
    float tex_step;
    
    if (!game->cfg.player.ray.hit.side)
        line_height = (int)(WINDOW_HEIGHT / game->cfg.player.ray.distance_x);
    else
        line_height = (int)(WINDOW_HEIGHT / game->cfg.player.ray.distance_y);
    tex_x = get_wall_hit(game, texture);
    tex_step = (float)texture.height / line_height;
    draw_start = (WINDOW_HEIGHT / 2) - (line_height / 2);
    draw_end = (WINDOW_HEIGHT / 2) + (line_height / 2);
    tex_y = 0;
    if (draw_start < 0)
    {
        tex_y = (-draw_start) * tex_step;
        draw_start = 0;
    }
    while (draw_start <= draw_end)
    {    
        my_mlx_pixel_put(&game->frame, x, draw_start, 
            get_texture_color(texture, (int)tex_y, tex_x));
        tex_y += tex_step;
        draw_start++;
    }
}
