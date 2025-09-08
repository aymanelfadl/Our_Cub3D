#include "cub3D.h"

void render(t_game game)
{
    int x = 0;

    while (x < WINDOW_WIDTH)
    {
        float camera_x = 2.0f * x / (float)WINDOW_WIDTH - 1.0f;
        float rayX = game.cfg.player.dir_x + camera_x * game.cfg.player.plane_x;
        float rayY = game.cfg.player.dir_y + camera_x * game.cfg.player.plane_y;
        printf("ray:(%.2f, %.2f)\n", rayX, rayY);

        // TODO: DDA >:

        x++;
    }
}



void start_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");

    render(*game);
}