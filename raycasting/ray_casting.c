#include "cub3D.h"

int load_textures(t_game *game)
{
    for (int i = 0; i < TEXTURE_COUNT; i++)
    {
        if (!game->cfg.textures[i].path)
            continue;

        game->cfg.textures[i].img.mlx_img = mlx_xpm_file_to_image(
            game->mlx,
            game->cfg.textures[i].path,
            &game->cfg.textures[i].img.width,
            &game->cfg.textures[i].img.height);
        if (!game->cfg.textures[i].img.mlx_img)
            return 0;

        game->cfg.textures[i].img.addr = mlx_get_data_addr(
            game->cfg.textures[i].img.mlx_img,
            &game->cfg.textures[i].img.bpp,
            &game->cfg.textures[i].img.line_len,
            &game->cfg.textures[i].img.endian);

        if (!game->cfg.textures[i].img.addr)
            return 0;
    }
    return 1;
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
        draw_vertical_line(game, x);
        x++;
    }
    draw_center(game, 8, 0);
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
}

int start_game(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (printf("mlx_init failed"), 0);

    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
    if (!game->win)
        return (printf("mlx_new_window failed"), 0);

    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->frame.mlx_img)
        return (printf("mlx_new_image failed"), 0);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);

    if (!load_textures(game))
        return (printf("Error: \nfailed to load textures\n"), 0);
    
    render(game);
    mlx_hook(game->win, 2, 1L << 0, handle_key, game);
    mlx_hook(game->win, 17, 0, close_game, game);
    mlx_loop(game->mlx);
    return 1;
}
