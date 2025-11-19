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

unsigned int get_texture_color(t_img texture, float tex_position, int tex_x)
{
    int tex_y;
    size_t offset;
    
    tex_y = (int)tex_position;
    if (tex_y < 0)
        tex_y = 0;
    if (tex_y >= texture.height)
        tex_y = texture.height - 1;
    offset = (size_t)tex_y * (size_t)texture.line_len + (size_t)tex_x * (texture.bpp / 8);
    return (*(unsigned int *)(texture.addr + offset));
}

void draw_vertical_line(t_game *game, int x)
{
    int line_height;
    int draw_start;
    int draw_end;
    int tex_x;
    t_img texture;
    float text_position;
    float step;
    
    texture = get_texture(game);
    line_height = (int)(WINDOW_HEIGHT / get_dist(game, game->cfg.player.ray.hit.side));
    draw_start = get_drawing_start(line_height);
    draw_end = get_drawing_end(line_height);
    tex_x = (int)(get_wall_hit(game, get_dist(game, game->cfg.player.ray.hit.side)) * (float)texture.width);
    step = (float)texture.height / line_height;
    text_position = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
    while (draw_start <= draw_end)
    {    
        text_position += step;
        my_mlx_pixel_put(&game->frame, x, draw_start, get_texture_color(texture, text_position, tex_x));
        draw_start++;
    }
}
