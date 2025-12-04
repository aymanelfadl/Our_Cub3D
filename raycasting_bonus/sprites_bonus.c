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

void draw_sprite_column(t_game *game, int screen_x, int start_y, int end_y, 
                       t_img texture, int tex_x, float depth)
{
    float tex_step;
    float tex_y;
    int y;
    
    // Skip if behind wall
    if (depth > game->z_buffer[screen_x])
        return;
    
    // Calculate texture step
    tex_step = (float)texture.height / (end_y - start_y);
    tex_y = 0;
    
    // Adjust if clipped at top
    if (start_y < 0)
    {
        tex_y = -start_y * tex_step;
        start_y = 0;
    }
    if (end_y >= WINDOW_HEIGHT)
        end_y = WINDOW_HEIGHT - 1;
    
    // Draw the column
    y = start_y;
    while (y < end_y)
    {
        int tex_y_int = (int)tex_y;
        unsigned int color = get_texture_color(texture, tex_y_int, tex_x);
        
        // Skip transparent pixels (black = 0x000000)
        if (color != 0x000000)
            my_mlx_pixel_put(&game->frame, screen_x, y, color);
        
        tex_y += tex_step;
        y++;
    }
}
void draw_sprite_textured(t_game *game, int i)
{
    float spriteX = game->sprites[i].x - game->cfg.player.pos_x;
    float spriteY = game->sprites[i].y - game->cfg.player.pos_y;

    float Det =  (game->cfg.player.plane_x * game->cfg.player.dir_y) - (game->cfg.player.dir_x * game->cfg.player.plane_y);
    float invDet = 1.0 / Det;

    float side = invDet * (game->cfg.player.dir_y * spriteX 
                               - game->cfg.player.dir_x * spriteY);
    float depth = invDet * (-game->cfg.player.plane_y * spriteX 
                               + game->cfg.player.plane_x * spriteY);

    if (depth <= 0)
        return;

    float projectedX = side / depth;

    int screenX = (int)((WINDOW_WIDTH / 2) * (1 + projectedX));

    int spriteHeight = abs((int)(WINDOW_HEIGHT / depth));

    int drawStartY = -spriteHeight / 2 + WINDOW_HEIGHT / 2;
    if (drawStartY < 0)
        drawStartY = 0;

    int drawEndY = spriteHeight / 2 + WINDOW_HEIGHT / 2;
    if (drawEndY >= WINDOW_HEIGHT)
        drawEndY = WINDOW_HEIGHT - 1;

    int spriteWidth = spriteHeight;

    int drawStartX = -spriteWidth / 2 + screenX;
    if (drawStartX < 0)
        drawStartX = 0;

    int drawEndX = spriteWidth / 2 + screenX;
    if (drawEndX >= WINDOW_WIDTH)
        drawEndX = WINDOW_WIDTH - 1;

    t_img texture = game->cfg.sprite_texture.img;

    for (int stripe = drawStartX; stripe < drawEndX; stripe++)
    {
        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + screenX))
                        * texture.width / spriteWidth) / 256;

        draw_sprite_column(game, stripe, drawStartY, drawEndY, texture, texX, depth);
    }
}

void draw_sprites(t_game *game)
{
    int i;

    i = 0;
    calculate_sprite_distances(game);
    sort_sprites(game->sprites, game->sprite_count);
    while (i < game->sprite_count)
    {
        draw_sprite_textured(game, i);
        i++;
    }
}