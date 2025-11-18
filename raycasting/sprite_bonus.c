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
    if ((color & 0xFF000000) == 0xFF000000)
        return (1);
    if (color == 0x000000)
        return (1);
    if ((color & 0x00FFFFFF) == 0x00FF00FF)
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

void draw_sprite(t_game *game, t_sprite *sprite)
{
    float sprite_x;
    float sprite_y;
    float inv_det;
    float transform_x;
    float transform_y;
    int sprite_screen_x;
    int sprite_height;
    int sprite_width;
    int draw_start_y;
    int draw_end_y;
    int draw_start_x;
    int draw_end_x;
    int stripe;
    int y;
    int tex_x;
    int tex_y;
    unsigned int color;
    
    // Step 1: Transform sprite to camera space
    sprite_x = sprite->x - game->cfg.player.pos_x;
    sprite_y = sprite->y - game->cfg.player.pos_y;
    
    inv_det = 1.0f / (game->cfg.player.plane_x * game->cfg.player.dir_y - 
                      game->cfg.player.dir_x * game->cfg.player.plane_y);
    
    transform_x = inv_det * (game->cfg.player.dir_y * sprite_x - 
                             game->cfg.player.dir_x * sprite_y);
    transform_y = inv_det * (-game->cfg.player.plane_y * sprite_x + 
                             game->cfg.player.plane_x * sprite_y);
    
    // Step 2: Skip if sprite is behind camera
    if (transform_y <= 0)
        return;
    
    // Step 3: Calculate screen position and size
    sprite_screen_x = (int)((WINDOW_WIDTH / 2) * (1 + transform_x / transform_y));
    sprite_height = abs((int)(WINDOW_HEIGHT / transform_y));
    sprite_width = sprite_height;

    draw_start_y = -sprite_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_start_y < 0)
        draw_start_y = 0;
    draw_end_y = sprite_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_end_y >= WINDOW_HEIGHT)
        draw_end_y = WINDOW_HEIGHT - 1;

    draw_start_x = -sprite_width / 2 + sprite_screen_x;
    if (draw_start_x < 0)
        draw_start_x = 0;
    draw_end_x = sprite_width / 2 + sprite_screen_x;
    if (draw_end_x >= WINDOW_WIDTH)
        draw_end_x = WINDOW_WIDTH - 1;
    
    // Step 4: Draw sprite columns with texture
    stripe = draw_start_x;
    while (stripe <= draw_end_x)
    {
        if (stripe < 0 || stripe >= WINDOW_WIDTH)
        {
            stripe++;
            continue;
        }
        
        if (transform_y >= game->z_buffer[stripe])
        {
            stripe++;
            continue;
        }
        
        // Draw column
        y = draw_start_y;
        while (y <= draw_end_y)
        {
            // If texture loaded, use it
            if (game->sprite_texture.addr != NULL)
            {
                tex_x = (int)((stripe - (-sprite_width / 2 + sprite_screen_x)) * 
                              game->sprite_texture.width / sprite_width);
                tex_y = (int)((y - draw_start_y) * game->sprite_texture.height / sprite_height);
                
                color = get_sprite_pixel(&game->sprite_texture, tex_x, tex_y);
                
                if (!is_transparent(color))
                    my_mlx_pixel_put(&game->frame, stripe, y, color);
            }
            else
            {
                // Fallback: draw green rectangle if no texture
                my_mlx_pixel_put(&game->frame, stripe, y, 0x00FF00);
            }
            
            y++;
        }
        stripe++;
    }
}