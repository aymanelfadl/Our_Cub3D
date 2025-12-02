#include "cub3D.h"


int check_door(t_game *game, float x, float y)
{
    t_door *door;

    door = find_door_at(game, (int)(x - COLLISION_CONST), (int)(y - COLLISION_CONST));
    if (door != NULL && !door->is_open)
        return (1);
    door = find_door_at(game, (int)(x + COLLISION_CONST), (int)(y - COLLISION_CONST));
    if (door != NULL && !door->is_open)
        return (1);
    door = find_door_at(game, (int)(x - COLLISION_CONST), (int)(y + COLLISION_CONST));
    if (door != NULL && !door->is_open)
        return (1);
    door = find_door_at(game, (int)(x + COLLISION_CONST), (int)(y + COLLISION_CONST));
    if (door != NULL && !door->is_open)
        return (1);
    return (0);
}

static int is_blocked(t_game *game, float x, float y)
{
    if (game->cfg.map.grid[(int)(y - COLLISION_CONST)][(int)(x - COLLISION_CONST)] == '1')
        return (1);
    if (game->cfg.map.grid[(int)(y - COLLISION_CONST)][(int)(x + COLLISION_CONST)] == '1')
        return (1);
    if (game->cfg.map.grid[(int)(y + COLLISION_CONST)][(int)(x - COLLISION_CONST)] == '1')
        return (1);
    if (game->cfg.map.grid[(int)(y + COLLISION_CONST)][(int)(x + COLLISION_CONST)] == '1')
        return (1);
    if (check_door(game, x, y))
        return (1);

    return (0);
}

void apply_movement(t_game *game, float new_x, float new_y)
{
    float old_x;
    float old_y;

    old_x = game->cfg.player.pos_x;
    old_y = game->cfg.player.pos_y;

    if (!is_blocked(game, new_x, new_y))
    {
        game->cfg.player.pos_x = new_x;
        game->cfg.player.pos_y = new_y;
        return;
    }
    if (!is_blocked(game, new_x, old_y))
    {
        game->cfg.player.pos_x = new_x;
        return;
    }
    if (!is_blocked(game, old_x, new_y))
    {
        game->cfg.player.pos_y = new_y;
        return;
    }
}

int	color_to_int(t_color c)
{
	return ((c.red << 16) | (c.green << 8) | c.blue);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;

    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

t_img get_proper_texture(t_texture *texs, t_direction dir)
{
    int i;
    t_img rst;
    
    rst.addr = NULL;
    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (texs[i].id == dir)
        {
            rst = texs[i].img;
            break;
        }
        i++;
    }
    return rst;
}