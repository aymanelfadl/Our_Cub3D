#include "cub3D.h"


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
                                    "textures/pillar.xpm",
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

    // Load door texture (use one of your wall textures or a custom door texture)
    game->door_texture.mlx_img = mlx_xpm_file_to_image(game->mlx,
                                    "textures/floor_1.xpm",  // Or use "textures/north.xpm" as fallback
                                    &game->door_texture.width,
                                    &game->door_texture.height);
    if (game->door_texture.mlx_img)
    {
        game->door_texture.addr = mlx_get_data_addr(game->door_texture.mlx_img,
                                    &game->door_texture.bpp,
                                    &game->door_texture.line_len,
                                    &game->door_texture.endian);
        printf("Door texture loaded: %dx%d\n", game->door_texture.width, game->door_texture.height);
    }
    else
    {
        printf("Warning: Could not load door texture\n");
        game->door_texture.addr = NULL;
    }

    // Create hardcoded doors for testing
    game->door_count = 2;
    game->doors = malloc(sizeof(t_door) * game->door_count);
    if (game->doors)
    {
        // Place doors at specific map positions (adjust to your map)
        game->doors[0].map_x = 37;  // Door 1 position
        game->doors[0].map_y = 5;
        game->doors[0].is_open = 0;  // Start closed
        
        game->doors[1].map_x = 39;  // Door 2 position
        game->doors[1].map_y = 7;
        game->doors[1].is_open = 1;  // Start open (to test)
        
        printf("Created %d doors\n", game->door_count);
    }

    render(game);
    mlx_hook(game->win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->win, 6,  1L<<6, mouse_move, game);
    mlx_hook(game->win, 17, 0, close_game, game);
    mlx_mouse_hide(game->mlx,game->win);
    mlx_loop(game->mlx);
    return 1;
}
