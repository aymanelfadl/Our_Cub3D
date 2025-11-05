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

int close_game(t_game *game)
{
    ft_free_textures_img(game);
    end_game(game);
    exit(EXIT_SUCCESS);
}

int handle_key(int key, t_game *game)
{
    printf("keycode:%d\n", key);
    player_movement(key, game);
    if (key == 65363)
        rotate_player(game, ROT_SPEED);
    if (key == 65361)
        rotate_player(game, -ROT_SPEED);
    if (key == 65307)
    {
        ft_free_textures_img(game);
        end_game(game);
        exit(EXIT_SUCCESS);
    }
    mlx_destroy_image(game->mlx, game->frame.mlx_img);
    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);
    render(game);
    return (0);
}
