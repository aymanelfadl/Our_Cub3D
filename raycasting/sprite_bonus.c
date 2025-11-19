#include "cub3D.h"

unsigned int get_sprite_pixel(t_img *texture, int x, int y)
{
    size_t offset;

    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
        return (0x000000);

    offset = (size_t)y * (size_t)texture->line_len + (size_t)x * (texture->bpp / 8);
    return (*(unsigned int *)(texture->addr + offset));
}

int is_transparent(unsigned int color)
{
    if (((color & 0xFF000000) == 0xFF000000))
        return (1);
    return (0);
}


void calculate_sprite_distances(t_game *game)
{
    int i;
    float dx;
    float dy;

    i = 0;
    while (i < game->sprite_count)
    {
        dx = game->cfg.player.pos_x - game->sprites[i].x;
        dy = game->cfg.player.pos_y - game->sprites[i].y;
        game->sprites[i].distance = sqrtf(dx * dx + dy * dy);
        i++;
    }
}

void sort_sprites(t_sprite *sprites, int count)
{
    int i;
    int j;
    t_sprite temp;

    i = 0;
    while (i < count - 1)
    {
        j = i + 1;
        while (j < count)
        {
            if (sprites[i].distance < sprites[j].distance)
            {
                temp = sprites[i];
                sprites[i] = sprites[j];
                sprites[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void update_sprite_animations(t_game *game)
{
    int i;

    i = 0;
    while (i < game->sprite_count)
    {
        // Update frame timer
        game->sprites[i].frame_timer += 0.01f;  // ~60 FPS (1/60 = 0.016)
        
        // Check if it's time to switch frames
        if (game->sprites[i].frame_timer >= game->sprites[i].frame_duration)
        {
            game->sprites[i].current_frame++;
            game->sprites[i].frame_timer = 0.0f;

            if (game->sprites[i].current_frame >= game->sprites[i].frame_count)
                game->sprites[i].current_frame = 0;
        }
        i++;
    }
}

int convert_to_camera_coordinate(t_game *game, t_sprite *sprite, float *transform_x, float *transform_y)
{
    float sprite_x;
    float sprite_y;
    float determinant;
    
    sprite_x = sprite->x - game->cfg.player.pos_x;
    sprite_y = sprite->y - game->cfg.player.pos_y;    
    determinant = game->cfg.player.plane_x * game->cfg.player.dir_y - game->cfg.player.dir_x * game->cfg.player.plane_y;
    *transform_x = (game->cfg.player.dir_y * sprite_x - game->cfg.player.dir_x * sprite_y) / determinant;
    *transform_y = (-game->cfg.player.plane_y * sprite_x + game->cfg.player.plane_x * sprite_y) / determinant;
    if (*transform_y <= 0)
        return (0);
    return (1);
}
    
float get_drawing_x(float transform_x, float transform_y, int is_start)
{
    int sprite_screen_x;
    int draw_x;
    int sprite_width;

    sprite_screen_x = (int)((WINDOW_WIDTH / 2) * (1 + transform_x / transform_y));
    sprite_width = (int)(WINDOW_HEIGHT / transform_y);
    if (is_start)
    {
        draw_x = -sprite_width / 2 + sprite_screen_x;
        if (draw_x < 0)
            return (0);
    }
    else
    {
        draw_x = sprite_width / 2 + sprite_screen_x;
        if (draw_x >= WINDOW_WIDTH)
            return (WINDOW_WIDTH - 1);
    }
    return (draw_x);
}

float get_drawing_y(float transform_y, int is_start)
{
    int draw_y;
    int sprite_height;
    
    sprite_height = (int)(WINDOW_HEIGHT / transform_y);
    if (is_start)
    {
        draw_y = -sprite_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_y < 0)
             return (0);
    }
    else
    {
        draw_y = sprite_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_y >= WINDOW_HEIGHT)
            return (WINDOW_HEIGHT - 1);
    }
    return (draw_y);
}

void init_sprite_render(t_sprite_render *render, float transform_x, float transform_y)
{
    render->sprite_height = (int)(WINDOW_HEIGHT / transform_y);
    render->sprite_width = render->sprite_height;
    render->sprite_screen_x = (int)((WINDOW_WIDTH / 2) * (1 + transform_x / transform_y));
    render->draw_start_x = get_drawing_x(transform_x, transform_y, 1);
    render->draw_end_x = get_drawing_x(transform_x, transform_y, 0);
    render->draw_start_y = get_drawing_y(transform_y, 1);
    render->draw_end_y = get_drawing_y(transform_y, 0);
}

void draw_sprite_pixel(t_game *game, t_sprite *sprite, t_sprite_render *render, int x, int y)
{
    int tex_x;
    int tex_y;
    int frame_index;
    unsigned int color;

    frame_index = sprite->current_frame;
    if (frame_index >= game->sprite_frame_count)
        frame_index = 0;
    
    tex_x = (int)((x - (-render->sprite_width / 2 + render->sprite_screen_x)) * game->sprite_textures[frame_index].width / render->sprite_width);
    tex_y = (int)((y - render->draw_start_y) * game->sprite_textures[frame_index].height / render->sprite_height);
    
    color = get_sprite_pixel(&game->sprite_textures[frame_index], tex_x, tex_y);
    
    if (!is_transparent(color))
        my_mlx_pixel_put(&game->frame, x, y, color);
}

void draw_sprite_column(t_game *game, t_sprite *sprite, t_sprite_render *render, int x)
{
    int y;

    y = render->draw_start_y;
    while (y <= render->draw_end_y)
    {
        draw_sprite_pixel(game, sprite, render, x, y);
        y++;
    }
}

void draw_sprite(t_game *game, t_sprite *sprite)
{
    float transform_x;
    float transform_y;
    t_sprite_render render;
    int x;

    if (!convert_to_camera_coordinate(game, sprite, &transform_x, &transform_y))
        return;
    
    init_sprite_render(&render, transform_x, transform_y);

    x = render.draw_start_x;
    while (x <= render.draw_end_x)
    {
        if (x >= 0 && x < WINDOW_WIDTH && transform_y < game->z_buffer[x])
            draw_sprite_column(game, sprite, &render, x);
        x++;
    }
}
