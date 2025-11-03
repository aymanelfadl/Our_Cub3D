#include "cub3D.h"
#include <mlx.h>

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;

    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}


void draw_vertical_line(t_game *game, int x, int drawStart, int drawEnd, int color)
{
    for (int y = drawStart; y <= drawEnd; y++)
    {
        if (y >= 0 && y < WINDOW_HEIGHT)
            my_mlx_pixel_put(&game->frame, x, y, color);
    }
}


void render(t_game *game)
{
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        // 1. Compute ray direction for this column
        float camera_x = 2.0f * x / (float)WINDOW_WIDTH - 1.0f;
        float rayX = game->cfg.player.dir_x + camera_x * game->cfg.player.plane_x;
        float rayY = game->cfg.player.dir_y + camera_x * game->cfg.player.plane_y;

        // 2. Current map square
        int map_x = (int)floorf(game->cfg.player.pos_x);
        int map_y = (int)floorf(game->cfg.player.pos_y);

        // 3. Distance the ray must travel to cross a grid line            
        float deltaDist_x, deltaDist_y;
        if (rayX != 0)
            deltaDist_x = fabsf(1.0f / rayX);
        else
            deltaDist_x = 1e30f;

        if (rayY != 0)
            deltaDist_y = fabsf(1.0f / rayY);
        else
            deltaDist_y = 1e30f;

        // 4. Initial side distances and step direction
        float sideDist_x, sideDist_y;
        int step_x, step_y;

        if (rayX < 0)
        {
            step_x = -1;
            sideDist_x = (game->cfg.player.pos_x - map_x) * deltaDist_x;
        }
        else
        {
            step_x = 1;
            sideDist_x = (map_x + 1.0f - game->cfg.player.pos_x) * deltaDist_x;
        }

        if (rayY < 0)
        {
            step_y = -1;
            sideDist_y = (game->cfg.player.pos_y - map_y) * deltaDist_y;
        }
        else
        {
            step_y = 1;
            sideDist_y = (map_y + 1.0f - game->cfg.player.pos_y) * deltaDist_y;
        }

        // 5. Perform DDA
        t_hit hit = {0};
        while (!hit.is_hit)
        {
            if (sideDist_x < sideDist_y)
            {
                sideDist_x += deltaDist_x;
                map_x += step_x;
                hit.side = 0;  // vertical wall
            }
            else
            {
                sideDist_y += deltaDist_y;
                map_y += step_y;
                hit.side = 1;  // horizontal wall
            }

            // Check if ray hit a wall
            if (game->cfg.map.grid[map_y][map_x] == '1')
                hit.is_hit = 1;
        }

        // 6. Compute perpendicular distance to avoid fisheye
        float perpWallDist;
        if (hit.side == 0)
            perpWallDist = (map_x - game->cfg.player.pos_x + (1 - step_x) / 2.0f) / rayX;
        else
            perpWallDist = (map_y - game->cfg.player.pos_y + (1 - step_y) / 2.0f) / rayY;


        // 7. Compute exact hit coordinates
        hit.hit_x = game->cfg.player.pos_x + rayX * perpWallDist;
        hit.hit_y = game->cfg.player.pos_y + rayY * perpWallDist;

        // 8. From here, you can calculate line height for rendering
        int lineHeight = (int)(WINDOW_HEIGHT / perpWallDist);
        int startLine = -lineHeight / 2 + WINDOW_HEIGHT / 2;
        int endLine = lineHeight / 2 + WINDOW_HEIGHT / 2;
        if (hit.side)
            draw_vertical_line(game, x, startLine, endLine, 0xFF0000);
        else
            draw_vertical_line(game, x, startLine, endLine, 0x0000FF);
        
        // Optional: print for debug
        printf("ray %d: hit at (%.2f, %.2f), side=%d, dist=%.2f, lineHeight=%d\n",
               x, hit.hit_x, hit.hit_y, hit.side, perpWallDist, lineHeight);
    }
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
}

void start_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");

    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);

    render(game);

    mlx_loop(game->mlx);
}