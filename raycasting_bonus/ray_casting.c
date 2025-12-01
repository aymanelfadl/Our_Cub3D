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
            color = color_to_int(game->cfg.floor);
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

int render_loop(t_game *game)
{
    render(game);
    return (0);
}

void draw_sprites(t_game *game)
{
    int i;
    
    if (game->sprite_count > 0)
    {
        update_sprite_animations(game);
        calculate_sprite_distances(game);
        sort_sprites(game->sprites, game->sprite_count);
        i = 0;
        while (i < game->sprite_count)
        {
            draw_sprite(game, &game->sprites[i]);
            i++;
        }
    }
}

void render(t_game *game)
{
    int x;
    int map_x;
    int map_y;
    int line_height;

    draw_background(game, color_to_int(game->cfg.ceiling), color_to_int(game->cfg.floor));
    x = 0;
    print_player_state(game);
    while (x < WINDOW_WIDTH)
    {
        map_x = floor(game->cfg.player.pos_x);
        map_y = floor(game->cfg.player.pos_y);
        compute_ray_direction(game, x);
        init_dda(game, map_y, map_x);
        perform_dda(game, &map_y, &map_x);
        if (!game->cfg.player.ray.hit.side)
        {
            game->z_buffer[x] = game->cfg.player.ray.distance_x;
            line_height = WINDOW_HEIGHT / game->cfg.player.ray.distance_x;
        }
        else
        {
            game->z_buffer[x] = game->cfg.player.ray.distance_y;
            line_height = WINDOW_HEIGHT / game->cfg.player.ray.distance_y;
        }
        x++;
    }
    draw_sprites(game);
    draw_minimap(game);
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
    mlx_put_image_to_window(game->mlx, game->win, game->minimap.mlx_img, 10, 10);
}

int start_game(t_game *game)
{
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

    render(game);

    mlx_loop_hook(game->mlx, render_loop, game);
    mlx_hook(game->win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->win, 6,  1L<<6, mouse_move, game);
    mlx_hook(game->win, 17, 0, close_game, game);
    mlx_mouse_hide(game->mlx,game->win);
    mlx_loop(game->mlx);
    return 1;
}