#include "cub3D.h"

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;

    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

void draw_vertical_line(t_game *game, int x)
{
    int lineHeight;
    float wall_x;
    if (!game->cfg.player.ray.hit.side)
    {
        lineHeight = (int)(WINDOW_HEIGHT / (game->cfg.player.ray.distance_x - game->cfg.player.ray.next_cell_x));
        wall_x = game->cfg.player.pos_y  + (game->cfg.player.ray.distance_x - game->cfg.player.ray.next_cell_x) * game->cfg.player.ray.ray_y;
    }
    else
    {
        lineHeight = (int)(WINDOW_HEIGHT / (game->cfg.player.ray.distance_y - game->cfg.player.ray.next_cell_y));
        wall_x = game->cfg.player.pos_x  + (game->cfg.player.ray.distance_y - game->cfg.player.ray.next_cell_y) * game->cfg.player.ray.ray_x;
    }


    
    wall_x -= floor(wall_x);
    int tex = (int)(wall_x * (float)texture->width);

    int startLine = -lineHeight / 2 + WINDOW_HEIGHT / 2;
    int endLine = lineHeight / 2 + WINDOW_HEIGHT / 2;

    float step = (float)texture->height / lineHeight;
    float pos_tex = (startLine - WINDOW_HEIGHT / 2 + lineHeight / 2) * step; 

    while (startLine <= endLine)
    {
        int tex_y = (int)pos_tex & (texture->height - 1);
        pos_tex += step;
        int color = *(unsigned int *)(texture->addr + (tex_y * texture->line_len + tex * (texture->bpp / 8)));
        my_mlx_pixel_put(&game->frame, x, startLine, color);
        startLine++;
    }

}
void compute_ray_direction(t_game *game, int column)
{
    game->cfg.player.ray.camera_x = 2.0f * column / (float)WINDOW_WIDTH - 1.0f;
    game->cfg.player.ray.ray_x = game->cfg.player.dir_x + game->cfg.player.ray.camera_x * game->cfg.player.plane_x;
    game->cfg.player.ray.ray_y = game->cfg.player.dir_y + game->cfg.player.ray.camera_x * game->cfg.player.plane_y;

    if (game->cfg.player.ray.ray_x != 0)
        game->cfg.player.ray.next_cell_x = fabsf(1.0f / game->cfg.player.ray.ray_x);
    else
        game->cfg.player.ray.next_cell_x = 1e30f;

    if (game->cfg.player.ray.ray_y != 0)
        game->cfg.player.ray.next_cell_y = fabsf(1.0f / game->cfg.player.ray.ray_y);
    else
        game->cfg.player.ray.next_cell_y = 1e30f;
}

void init_dda(t_game *game, int map_y, int map_x)
{
    if (game->cfg.player.ray.ray_x < 0)
    {
        game->cfg.player.ray.step_x = -1;
        game->cfg.player.ray.distance_x = (game->cfg.player.pos_x - map_x) * game->cfg.player.ray.next_cell_x;
    }
    else
    {
        game->cfg.player.ray.step_x = 1;
        game->cfg.player.ray.distance_x = (map_x + 1.0f - game->cfg.player.pos_x) * game->cfg.player.ray.next_cell_x;
    }

    if (game->cfg.player.ray.ray_y < 0)
    {
        game->cfg.player.ray.step_y = -1;
        game->cfg.player.ray.distance_y = (game->cfg.player.pos_y - map_y) * game->cfg.player.ray.next_cell_y;
    }
    else
    {
        game->cfg.player.ray.step_y = 1;
        game->cfg.player.ray.distance_y = (map_y + 1.0f - game->cfg.player.pos_y) * game->cfg.player.ray.next_cell_y;
    }
}

void perform_dda(t_game *game, int *map_y, int *map_x)
{
    game->cfg.player.ray.hit.is_hit = 0;

    while (!game->cfg.player.ray.hit.is_hit)
    {
        if (game->cfg.player.ray.distance_x < game->cfg.player.ray.distance_y)
        {
            game->cfg.player.ray.distance_x += game->cfg.player.ray.next_cell_x;
            (*map_x) += game->cfg.player.ray.step_x;
            game->cfg.player.ray.hit.side = 0; // vertical wall
        }
        else
        {
            game->cfg.player.ray.distance_y += game->cfg.player.ray.next_cell_y;
            (*map_y) += game->cfg.player.ray.step_y;
            game->cfg.player.ray.hit.side = 1; // horizontal wall
        }

        if (game->cfg.map.grid[*map_y][*map_x] == '1')
            game->cfg.player.ray.hit.is_hit = 1;
    }

    // 7. Compute exact hit coordinates
    game->cfg.player.ray.hit.hit_x = game->cfg.player.pos_x + game->cfg.player.ray.ray_x * (game->cfg.player.ray.distance_x - game->cfg.player.ray.next_cell_x);
    game->cfg.player.ray.hit.hit_y = game->cfg.player.pos_y + game->cfg.player.ray.ray_y * (game->cfg.player.ray.distance_y - game->cfg.player.ray.next_cell_y);
}

int close_game(t_game *game)
{
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
}

void draw_center(t_game *game, int size, int color)
{
    int cx = WINDOW_WIDTH / 2;
    int cy = WINDOW_HEIGHT / 2;

    int x = cx - size;
    while (x <= cx + size)
    {
        my_mlx_pixel_put(&game->frame, x, cy, color);
        x++;
    }

    int y = cy - size;
    while (y <= cy + size)
    {
        my_mlx_pixel_put(&game->frame, cx, y, color);
        y++;
    }
}



void render(t_game *game)
{

    game->cfg.textures[0].img->mlx_img = mlx_xpm_file_to_image(game->mlx, game->cfg.textures[0].path,
                                            &game->cfg.textures[0].img->width, &game->cfg.textures[0].img->height);
    game->cfg.textures[0].img->addr = mlx_get_data_addr(texture->mlx_img,
                                    &texture->bpp,
                                    &texture->line_len,
                                    &texture->endian);

    for (int x = 0; x < WINDOW_WIDTH; x++)
    {

        int map_x = (int)floorf(game->cfg.player.pos_x);
        int map_y = (int)floorf(game->cfg.player.pos_y);

        compute_ray_direction(game, x);

        // 4. Initial side distances and step direction
        init_dda(game, map_y, map_x);

        // 5. Perform DDA
        perform_dda(game, &map_y, &map_x);

        // 8. From here, you can calculate line height for rendering
        draw_vertical_line(game, x);
    }
    draw_center(game, 8, 0);
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
}

void rotate_player(t_game *game, double angle)
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

int handle_key(int key, t_game *game)
{
    printf("keycode:%d\n", key);
    float x_move;
    float y_move;

    if (key == 119)
    {
        x_move = game->cfg.player.pos_x + game->cfg.player.dir_x * MOVE_SPEED;
        y_move = game->cfg.player.pos_y + game->cfg.player.dir_y * MOVE_SPEED;

        if (game->cfg.map.grid[(int)game->cfg.player.pos_y][(int)x_move] != '1')
            game->cfg.player.pos_x = x_move;
        if (game->cfg.map.grid[(int)y_move][(int)game->cfg.player.pos_x] != '1')
            game->cfg.player.pos_y = y_move;

    }
    if (key == 115)
    {
        x_move = game->cfg.player.pos_x - game->cfg.player.dir_x * MOVE_SPEED;
        y_move = game->cfg.player.pos_y - game->cfg.player.dir_y * MOVE_SPEED;

        if (game->cfg.map.grid[(int)y_move][(int)x_move] != '1')
        {
            game->cfg.player.pos_x = x_move;
            game->cfg.player.pos_y = y_move;
        }
    }
    if (key == 97)
        rotate_player(game, -ROT_SPEED);
    if (key == 100)
        rotate_player(game, ROT_SPEED);


    mlx_destroy_image(game->mlx, game->frame.mlx_img);
    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);
    render(game);

    return 0;
}


void start_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");

    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);

    render(game);
    mlx_hook(game->win, 2, 1L<<0, handle_key, game);
    mlx_hook(game->win, 17, 0, close_game, game);


    mlx_loop(game->mlx);
}