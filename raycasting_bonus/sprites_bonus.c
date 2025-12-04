#include "cub3D.h"

void calculate_sprite_distances(t_game *game)
{
    int i;
    float dx;
    float dy;

    i = 0;
    while (i < game->sprite_count)
    {
        dx = game->sprites[i].x - game->cfg.player.pos_x;
        dy = game->sprites[i].y - game->cfg.player.pos_y;
        game->sprites[i].distance = pow(dx, 2) + pow(dy, 2);
        i++;
    }
}

void sort_sprites(t_sprite *sprites, int count)
{
    int i;
    int j;
    t_sprite tmp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (sprites[j].distance < sprites[j + 1].distance)
            {
                tmp = sprites[j];
                sprites[j] = sprites[j + 1];
                sprites[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
}

void transform_to_camera_space(t_game *game, t_sprite *sprite)
{
    float sprite_x;
    float sprite_y;
    float det;
    float inv_det;
    float side;

    sprite_x = sprite->x - game->cfg.player.pos_x;
    sprite_y = sprite->y - game->cfg.player.pos_y;

    det = (game->cfg.player.plane_x * game->cfg.player.dir_y) - 
          (game->cfg.player.dir_x * game->cfg.player.plane_y);
    inv_det = 1.0 / det;
    
    side = inv_det * (game->cfg.player.dir_y * sprite_x - game->cfg.player.dir_x * sprite_y);
    
    sprite->render.depth = inv_det * (-game->cfg.player.plane_y * sprite_x + 
                                      game->cfg.player.plane_x * sprite_y);
    
    sprite->render.screen_x = (int)((WINDOW_WIDTH / 2) * (1 + side / sprite->render.depth));
    sprite->render.sprite_width = abs((int)(WINDOW_HEIGHT / sprite->render.depth));
}

void calculate_vertical_bounds(t_sprite_render *render)
{
    render->start_y = (WINDOW_HEIGHT - render->sprite_width) / 2;
    render->end_y = (WINDOW_HEIGHT + render->sprite_width) / 2;
    if (render->start_y < 0)
        render->start_y = 0;
    if (render->end_y >= WINDOW_HEIGHT)
        render->end_y = WINDOW_HEIGHT - 1;
}

void calculate_horizontal_bounds(t_sprite_render *render)
{
    render->sprite_left = render->screen_x - render->sprite_width / 2;
    render->start_x = render->sprite_left;
    render->end_x = render->screen_x + render->sprite_width / 2;
    if (render->start_x < 0)
        render->start_x = 0;
    if (render->end_x >= WINDOW_WIDTH)
        render->end_x = WINDOW_WIDTH - 1;
}

void draw_sprite_stripe(t_game *game, int x, t_sprite_render *render,
                       t_img texture, int tex_x)
{
    float tex_step;
    float tex_pos;
    int y;
    unsigned int color;

    if (render->depth > game->z_buffer[x])
        return;
    tex_step = (float)texture.height / (render->end_y - render->start_y);
    tex_pos = 0;
    if (render->start_y < 0)
    {
        tex_pos = -render->start_y * tex_step;
        render->start_y = 0;
    }
    if (render->end_y >= WINDOW_HEIGHT)
        render->end_y = WINDOW_HEIGHT - 1;
    y = render->start_y;
    while (y < render->end_y)
    {
        color = get_texture_color(texture, (int)tex_pos, tex_x);
        if (color != 0x000000)
            my_mlx_pixel_put(&game->frame, x, y, color);
        tex_pos += tex_step;
        y++;
    }
}

void draw_sprite_columns(t_game *game, t_sprite_render *render, t_img texture)
{
    int x;
    int offset_x;
    int tex_x;
    float tex_step_x;

    tex_step_x = (float)texture.width / render->sprite_width;
    x = render->start_x;
    while (x < render->end_x)
    {
        offset_x = x - render->sprite_left;
        tex_x = (int)(offset_x * tex_step_x);
        if (tex_x >= texture.width)
            tex_x = texture.width - 1;
        draw_sprite_stripe(game, x, render, texture, tex_x);
        x++;
    }
}

void render_sprite(t_game *game, int sprite_index)
{
    t_sprite *sprite;
    t_img texture;

    sprite = &game->sprites[sprite_index];
    transform_to_camera_space(game, sprite);
    if (sprite->render.depth <= 0)
        return;
    calculate_vertical_bounds(&sprite->render);
    calculate_horizontal_bounds(&sprite->render);
    texture = game->cfg.sprite_texture.img;
    draw_sprite_columns(game, &sprite->render, texture);
}

void draw_sprites(t_game *game)
{
    int i;

    calculate_sprite_distances(game);
    sort_sprites(game->sprites, game->sprite_count);
    i = 0;
    while (i < game->sprite_count)
    {
        render_sprite(game, i);
        i++;
    }
}