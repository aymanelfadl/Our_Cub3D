#include "cub3D.h"

// Calculate distance from player to each sprite
void calculate_sprite_distances(t_game *game)
{
    int i;

    i = 0;
    while (i < game->sprite_count)
    {
        float dx = game->sprites[i].x - game->cfg.player.pos_x;
        float dy = game->sprites[i].y - game->cfg.player.pos_y;
        
        // Store squared distance (no need for sqrt since we only compare)
        game->sprites[i].distance = dx * dx + dy * dy;
        i++;
    }
}

// Sort sprites from farthest to nearest (painter's algorithm)
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
            // Farthest first (larger distance first)
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

// Draw a single vertical stripe of the sprite
void draw_sprite_stripe(t_game *game, int screen_x, int start_y, int end_y,
                        t_img texture, int tex_x, float depth)
{
    float tex_step;
    float tex_pos;
    int y;
    
    // Don't draw if behind a wall (z-buffer check)
    if (depth > game->z_buffer[screen_x])
        return;
    
    // Calculate how much to advance in texture per screen pixel
    tex_step = (float)texture.height / (end_y - start_y);
    tex_pos = 0;
    
    // Handle clipping at screen edges
    if (start_y < 0)
    {
        tex_pos = -start_y * tex_step;  // Skip hidden texture pixels
        start_y = 0;
    }
    if (end_y >= WINDOW_HEIGHT)
        end_y = WINDOW_HEIGHT - 1;
    
    // Draw each pixel in this vertical stripe
    y = start_y;
    while (y < end_y)
    {
        int tex_y = (int)tex_pos;
        unsigned int color = get_texture_color(texture, tex_y, tex_x);
        
        // Skip transparent pixels (assuming black = transparent)
        if (color != 0x000000)
            my_mlx_pixel_put(&game->frame, screen_x, y, color);
        
        tex_pos += tex_step;
        y++;
    }
}

// Transform sprite from world space to camera space and render
void render_sprite(t_game *game, int sprite_index)
{
    // Step 1: Get sprite position relative to player
    float sprite_x = game->sprites[sprite_index].x - game->cfg.player.pos_x;
    float sprite_y = game->sprites[sprite_index].y - game->cfg.player.pos_y;

    // Step 2: Transform to camera space using inverse camera matrix
    float determinant = (game->cfg.player.plane_x * game->cfg.player.dir_y) - 
                        (game->cfg.player.dir_x * game->cfg.player.plane_y);

    float inv_det = 1.0 / determinant;

    // Camera space coordinates
    float cam_x = inv_det * (game->cfg.player.dir_y * sprite_x - 
                             game->cfg.player.dir_x * sprite_y);
    float cam_z = inv_det * (-game->cfg.player.plane_y * sprite_x + 
                             game->cfg.player.plane_x * sprite_y);

    // Step 3: Skip if sprite is behind camera
    if (cam_z <= 0)
        return;

    // Step 4: Project to screen coordinates
    float screen_x_ratio = cam_x / cam_z;  // Perspective division
    int screen_x = (int)((WINDOW_WIDTH / 2) * (1 + screen_x_ratio));

    // Step 5: Calculate sprite size on screen (closer = bigger)
    int sprite_height = abs((int)(WINDOW_HEIGHT / cam_z));
    int sprite_width = sprite_height;  // Square sprites

    // Step 6: Calculate drawing boundaries (vertical)
    int draw_start_y = (WINDOW_HEIGHT - sprite_height) / 2;
    int draw_end_y = (WINDOW_HEIGHT + sprite_height) / 2;
    
    if (draw_start_y < 0)
        draw_start_y = 0;
    if (draw_end_y >= WINDOW_HEIGHT)
        draw_end_y = WINDOW_HEIGHT - 1;

    // Step 7: Calculate drawing boundaries (horizontal)
    int sprite_left = screen_x - sprite_width / 2;
    int sprite_right = screen_x + sprite_width / 2;
    
    int draw_start_x = sprite_left;
    int draw_end_x = sprite_right;
    
    if (draw_start_x < 0)
        draw_start_x = 0;
    if (draw_end_x >= WINDOW_WIDTH)
        draw_end_x = WINDOW_WIDTH - 1;

    // Step 8: Get texture
    t_img texture = game->cfg.sprite_texture.img;
    
    // Step 9: Calculate texture mapping step (SIMPLIFIED!)
    float tex_step_x = (float)texture.width / sprite_width;

    // Step 10: Draw sprite column by column
    while (draw_start_x < draw_end_x)
    {
        // Calculate which texture column to use
        int offset_x = draw_start_x - sprite_left;  // How far into sprite are we?
        int tex_x = (int)(offset_x * tex_step_x);  // Map to texture
        
        // Clamp texture coordinate
        if (tex_x >= texture.width)
            tex_x = texture.width - 1;
        
        // Draw this vertical stripe
        draw_sprite_stripe(game, draw_start_x, draw_start_y, draw_end_y, 
                          texture, tex_x, cam_z);
        draw_start_x++;
    }
}

// Main sprite rendering function
void draw_sprites(t_game *game)
{
    int i;

    // Sort sprites by distance (farthest first)
    calculate_sprite_distances(game);
    sort_sprites(game->sprites, game->sprite_count);
    
    // Draw all sprites in order
    i = 0;
    while (i < game->sprite_count)
    {
        render_sprite(game, i);
        i++;
    }
}