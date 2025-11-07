#include "cub3D.h"
#include "parser.h"

/* Forward declarations for static/internal callbacks used by start_game */
static int  handle_key_press(int keycode, t_game *game);
static int  handle_close(t_game *game);
static int  move_player(t_game *game, float dx, float dy);
static void rotate_player(t_game *game, float angle);
static int  is_walkable(t_game *game, float x, float y);
static void shutdown_game(t_game *game, int exit_code);

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
    // Reset the frame buffer so old pixels do not linger between renders
    if (game->frame.addr)
        ft_bzero(game->frame.addr, (size_t)game->frame.line_len * WINDOW_HEIGHT);

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

        /* select texture based on wall orientation */
        int tex_index = 0;
        if (hit.side == 0)
        {
            if (rayX > 0)
                tex_index = WE;
            else
                tex_index = EA;
        }
        else
        {
            if (rayY > 0)
                tex_index = NO;
            else
                tex_index = SO;
        }

        t_texture *tex = &game->cfg.textures[tex_index];
        if (!tex->loaded || !tex->img.addr || tex->width <= 0 || tex->height <= 0)
        {
            /* fallback to solid color */
            if (hit.side)
                draw_vertical_line(game, x, startLine, endLine, 0xFF0000);
            else
                draw_vertical_line(game, x, startLine, endLine, 0x0000FF);
            continue;
        }

        /* calculate exact position on wall */
        float wallX = (hit.side == 0) ? hit.hit_y : hit.hit_x;
        wallX -= floorf(wallX);
        int tex_x = (int)(wallX * (float)tex->width);
        if (hit.side == 0 && rayX > 0)
            tex_x = tex->width - tex_x - 1;
        if (hit.side == 1 && rayY < 0)
            tex_x = tex->width - tex_x - 1;

        float step = (float)tex->height / (float)lineHeight;
        float texPos = (startLine - WINDOW_HEIGHT / 2 + lineHeight / 2) * step;
        for (int y = startLine; y <= endLine; ++y)
        {
            if (y < 0 || y >= WINDOW_HEIGHT)
            {
                texPos += step;
                continue;
            }
            int tex_y = (int)texPos;
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= tex->height) tex_y = tex->height - 1;
            unsigned int color = tex_get_pixel(&tex->img, tex_x, tex_y);
            if (hit.side)
                color = (color >> 1) & 0x7F7F7F; /* simple shading */
            my_mlx_pixel_put(&game->frame, x, y, color);
            texPos += step;
        }
        
    }
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
}

void start_game(t_game *game)
{
    if (!game->mlx)
        game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");

    game->frame.mlx_img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->frame.addr = mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);

#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    mlx_hook(game->win, EVENT_DESTROY, 0, (int (*)())handle_close, game);
    mlx_hook(game->win, EVENT_KEY_PRESS, MASK_KEY_PRESS, (int (*)())handle_key_press, game);
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic pop
#endif

    render(game);

    mlx_loop(game->mlx);
}

static int handle_key_press(int keycode, t_game *game)
{
    int updated;
    updated = 0;
    if (keycode == KEY_ESC)
        shutdown_game(game, 0);
    else if (keycode == KEY_W)
        updated = move_player(game, game->cfg.player.dir_x * MOVE_SPEED,
                              game->cfg.player.dir_y * MOVE_SPEED);
    else if (keycode == KEY_S)
        updated = move_player(game, -game->cfg.player.dir_x * MOVE_SPEED,
                              -game->cfg.player.dir_y * MOVE_SPEED);
    else if (keycode == KEY_Q)
        updated = move_player(game, -game->cfg.player.plane_x * MOVE_SPEED,
                              -game->cfg.player.plane_y * MOVE_SPEED);
    else if (keycode == KEY_E)
        updated = move_player(game, game->cfg.player.plane_x * MOVE_SPEED,
                              game->cfg.player.plane_y * MOVE_SPEED);
    else if (keycode == KEY_A || keycode == KEY_LEFT)
    {
        rotate_player(game, -ROT_SPEED);
        updated = 1;
    }
    else if (keycode == KEY_D || keycode == KEY_RIGHT)
    {
        rotate_player(game, ROT_SPEED);
        updated = 1;
    }
    if (updated)
        render(game);
    return (0);
}

static int handle_close(t_game *game)
{
    shutdown_game(game, 0);
    return (0);
}

static int move_player(t_game *game, float dx, float dy)
{
    int moved;

    moved = 0;
    if (is_walkable(game, game->cfg.player.pos_x + dx, game->cfg.player.pos_y))
    {
        game->cfg.player.pos_x += dx;
        moved = 1;
    }
    if (is_walkable(game, game->cfg.player.pos_x, game->cfg.player.pos_y + dy))
    {
        game->cfg.player.pos_y += dy;
        moved = 1;
    }
    return (moved);
}

static void rotate_player(t_game *game, float angle)
{
    float old_dir_x;
    float old_plane_x;
    float cos_angle;
    float sin_angle;

    cos_angle = cosf(angle);
    sin_angle = sinf(angle);
    old_dir_x = game->cfg.player.dir_x;
    game->cfg.player.dir_x = game->cfg.player.dir_x * cos_angle - game->cfg.player.dir_y * sin_angle;
    game->cfg.player.dir_y = old_dir_x * sin_angle + game->cfg.player.dir_y * cos_angle;
    old_plane_x = game->cfg.player.plane_x;
    game->cfg.player.plane_x = game->cfg.player.plane_x * cos_angle - game->cfg.player.plane_y * sin_angle;
    game->cfg.player.plane_y = old_plane_x * sin_angle + game->cfg.player.plane_y * cos_angle;
}

static int is_walkable(t_game *game, float x, float y)
{
    int map_x;
    int map_y;

    if (x < 0 || y < 0)
        return (0);
    map_x = (int)x;
    map_y = (int)y;
    if (map_y < 0 || map_y >= game->cfg.map.height)
        return (0);
    if (map_x < 0 || map_x >= game->cfg.map.width)
        return (0);
    if (game->cfg.map.grid[map_y] == NULL)
        return (0);
    if ((int)ft_strlen(game->cfg.map.grid[map_y]) <= map_x)
        return (0);
    if (game->cfg.map.grid[map_y][map_x] == '1' || game->cfg.map.grid[map_y][map_x] == ' ')
        return (0);
    return (1);
}

static void shutdown_game(t_game *game, int exit_code)
{
    if (game->frame.mlx_img)
        mlx_destroy_image(game->mlx, game->frame.mlx_img);
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    parser_release_config(&game->cfg);
    exit(exit_code);
}