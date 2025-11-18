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

void draw_player(t_game *game)
{
    int player_radius;
    int py;
    int center_x;
    int center_y;

    player_radius = 4;
    center_x = RADIUS * TILE_SIZE;
    center_y = RADIUS * TILE_SIZE;
    py = -player_radius;
    while (py <= player_radius)
    {
        int px = -player_radius;
        while (px <= player_radius)
        {
            if (px * px + py * py <= player_radius * player_radius)
                my_mlx_pixel_put(&game->minimap, center_x + px, center_y + py, 0xFF0000);
            px++;
        }
        py++;
    }
}

int get_tile_color(t_game *game, int map_x, int map_y)
{
    int color;
    
    color = 0x2C2D2D;
    if (map_y >= 0 && map_y < game->cfg.map.height && 
        map_x >= 0 && map_x < game->cfg.map.width)
    {
        if (game->cfg.map.grid[map_y][map_x] == '1')
            color = 0x565757;
        else if (game->cfg.map.grid[map_y][map_x] == '0')
            color = color_to_int(game->cfg.floor_color);
    }
    return (color);
}

void draw_tile(t_game *game, int screen_x, int screen_y, int color)
{
    int tile_y;
    int tile_x;
    int px;
    int py;

    tile_y = 0;
    while (tile_y < TILE_SIZE)
    {
        tile_x = 0;
        while (tile_x < TILE_SIZE)
        {
            px = screen_x * TILE_SIZE + tile_x;
            py = screen_y * TILE_SIZE + tile_y;
            my_mlx_pixel_put(&game->minimap, px, py, color);
            tile_x++;
        }
        tile_y++;
    }
}

void draw_minimap(t_game *game)
{
    int screen_y;
    int screen_x;
    int map_x;
    int map_y;

    map_y = (int)game->cfg.player.pos_y - RADIUS;
    screen_y = 0;
    while (screen_y < RADIUS * 2)
    {
        map_x = (int)game->cfg.player.pos_x - RADIUS;
        screen_x = 0;
        while (screen_x < RADIUS * 2)   
        {
            draw_tile(game, screen_x, screen_y, get_tile_color(game, map_x, map_y));
            map_x++;
            screen_x++;
        }
        map_y++;
        screen_y++;
    }
    draw_player(game);
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

void render(t_game *game)
{
    int x;
    int map_x;
    int map_y;

    draw_background(game, color_to_int(game->cfg.ceiling_color), color_to_int(game->cfg.floor_color));
    x = 0;
    while (x < WINDOW_WIDTH)
    {
        map_x = floor(game->cfg.player.pos_x);
        map_y = floor(game->cfg.player.pos_y);
        compute_ray_direction(game, x);
        init_dda(game, map_y, map_x);
        perform_dda(game, &map_y, &map_x);

        game->z_buffer[x] = get_dist(game, game->cfg.player.ray.hit.side);

        draw_vertical_line(game, x);
        x++;
    }
    
    if (game->sprite_count > 0)
    {
        calculate_sprite_distances(game);
        sort_sprites(game->sprites, game->sprite_count);
        int i = 0;
        while (i < game->sprite_count)
        {
            draw_sprite(game, &game->sprites[i]);
            i++;
        }
    }
    
    draw_minimap(game);
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
    mlx_put_image_to_window(game->mlx, game->win, game->minimap.mlx_img, 10, 10);
}

int start_game(t_game *game)
{
    if (!game->mlx)
        game->mlx = mlx_init();
    if (!game->mlx)
                return (fprintf(stderr, "Error\nFailed to initialize MLX\n"), 0);

    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
    if (!game->win)
        return (fprintf(stderr, "mlx_new_window failed\n"), 0);

    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->frame.mlx_img)
        return (fprintf(stderr, "mlx_new_image failed\n"), 0);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);

    game->minimap.mlx_img = mlx_new_image(game->mlx, 100, 100);
    if (!game->minimap.mlx_img)
        return (fprintf(stderr, "mlx_new_image failed for minimap\n"), 0);
    game->minimap.addr = mlx_get_data_addr(game->minimap.mlx_img, &game->minimap.bpp, &game->minimap.line_len, &game->minimap.endian);

    if (texture_load_all(game->mlx, &game->cfg) != 0)
        return (fprintf(stderr, "Error\nFailed to load textures\n"), 0);

    // Load sprite texture (hardcoded path - adjust as needed)
    game->sprite_texture.mlx_img = mlx_xpm_file_to_image(game->mlx, 
                                    "textures/sprite_1.xpm",
                                    &game->sprite_texture.width,
                                    &game->sprite_texture.height);
    if (game->sprite_texture.mlx_img)
    {
        game->sprite_texture.addr = mlx_get_data_addr(game->sprite_texture.mlx_img,
                                    &game->sprite_texture.bpp,
                                    &game->sprite_texture.line_len,
                                    &game->sprite_texture.endian);
        printf("Sprite texture loaded: %dx%d\n", game->sprite_texture.width, game->sprite_texture.height);
    }
    else
    {
        printf("Warning: Could not load sprite texture 'textures/barrel.xpm'\n");
        game->sprite_texture.addr = NULL;
    }

    // Create fake static sprites for testing
    // Place them in front of the player based on typical map positions
    game->sprite_count = 3;
    game->sprites = malloc(sizeof(t_sprite) * game->sprite_count);
    if (game->sprites)
    {
        // Adjust these positions to be near your player's starting location
        // Player is around (42.5, 5.5), so put sprites ahead
        game->sprites[0].x = 40.5f;  // Close to player
        game->sprites[0].y = 5.5f;
        game->sprites[0].distance = 0.0f;
        
        game->sprites[1].x = 38.5f;  // A bit further
        game->sprites[1].y = 7.5f;
        game->sprites[1].distance = 0.0f;
        
        game->sprites[2].x = 35.5f;  // Even further
        game->sprites[2].y = 5.5f;
        game->sprites[2].distance = 0.0f;
    }

    render(game);
    mlx_hook(game->win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->win, 6,  1L<<6, mouse_move, game);
    mlx_hook(game->win, 17, 0, close_game, game);
    mlx_mouse_hide(game->mlx,game->win);
    mlx_loop(game->mlx);
    return 1;
}
