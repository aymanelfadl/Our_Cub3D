#include "cub3D.h"

static void rotate_player(t_game *game, double angle)
{
    double old_dir_x = game->cfg.player.dir_x;
    double old_dir_y = game->cfg.player.dir_y;
    double old_plane_x = game->cfg.player.plane_x;

    // rotate direction vector
    game->cfg.player.dir_x = old_dir_x * cos(angle) - old_dir_y * sin(angle);
    game->cfg.player.dir_y = old_dir_x * sin(angle) + old_dir_y * cos(angle);

    // rotate camera plane
    game->cfg.player.plane_x = game->cfg.player.plane_x * cos(angle) - game->cfg.player.plane_y * sin(angle);
    game->cfg.player.plane_y = old_plane_x * sin(angle) + game->cfg.player.plane_y * cos(angle);
}

int close_game(t_game *game)
{
    if (game && game->mlx)
        texture_free_all(game->mlx, &game->cfg);
    if (game && game->frame.mlx_img)
        mlx_destroy_image(game->mlx, game->frame.mlx_img);
    if (game && game->win)
        mlx_destroy_window(game->mlx, game->win);
    parser_release_config(&game->cfg);
    exit(EXIT_SUCCESS);
}

int handle_key(int key, t_game *game)
{
    player_movement(key, game);
    if (key == KEY_RIGHT)
        rotate_player(game, ROT_SPEED);
    if (key == KEY_LEFT)
        rotate_player(game, -ROT_SPEED);
    if (key == KEY_ESC)
        close_game(game);
    mlx_destroy_image(game->mlx, game->frame.mlx_img);
    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);
    render(game);
    return (0);
}