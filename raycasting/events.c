#include "cub3D.h"

static void rotate_player(t_game *game, double angle)
{
    double old_dir_x = game->cfg.player.dir_x;
    double old_plane_x = game->cfg.player.plane_x;

    // rotate direction vector
    game->cfg.player.dir_x = game->cfg.player.dir_x * cos(angle) - game->cfg.player.dir_y * sin(angle);
    game->cfg.player.dir_y = old_dir_x * sin(angle) + game->cfg.player.dir_y * cos(angle);

    // rotate camera plane
    game->cfg.player.plane_x = game->cfg.player.plane_x * cos(angle) - game->cfg.player.plane_y * sin(angle);
    game->cfg.player.plane_y = old_plane_x * sin(angle) + game->cfg.player.plane_y * cos(angle);
}

int mouse_move(int x, int y, void *game)
{
    t_game *g;
    int new_x;
    static int  old_x;
    (void)y;

    g = (t_game *)game;
    new_x = x - old_x;
    old_x = x;
    if (new_x > 0)
        rotate_player(g, (ROT_SPEED - 0.02f));
    else
        rotate_player(g, -(ROT_SPEED - 0.02f));
    mlx_destroy_image(g->mlx, g->frame.mlx_img);
    g->frame.mlx_img = mlx_new_image(g->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    g->frame.addr = mlx_get_data_addr(g->frame.mlx_img, &g->frame.bpp, &g->frame.line_len, &g->frame.endian);
    render(game);
    return (0);
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
    if (key == 65363)
        rotate_player(game, ROT_SPEED);
    if (key == 65361)
        rotate_player(game, -ROT_SPEED);
    if (key == 65307)
        close_game(game);
    mlx_destroy_image(game->mlx, game->frame.mlx_img);
    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);
    render(game);
    return (0);
}
