#include "cub3D.h"

static void move_forward(t_game *game)
{
    float new_x = game->cfg.player.pos_x + game->cfg.player.dir_x * MOVE_SPEED;
    float new_y = game->cfg.player.pos_y + game->cfg.player.dir_y * MOVE_SPEED;

    if (!is_blocked(game, (int)new_x, (int)game->cfg.player.pos_y))
        game->cfg.player.pos_x = new_x;

    if (!is_blocked(game, (int)game->cfg.player.pos_x, (int)new_y))
        game->cfg.player.pos_y = new_y;
}

static void move_backward(t_game *game)
{
    float new_x = game->cfg.player.pos_x - game->cfg.player.dir_x * MOVE_SPEED;
    float new_y = game->cfg.player.pos_y - game->cfg.player.dir_y * MOVE_SPEED;

    if (!is_blocked(game, (int)new_x, (int)game->cfg.player.pos_y))
        game->cfg.player.pos_x = new_x;

    if (!is_blocked(game, (int)game->cfg.player.pos_x, (int)new_y))
        game->cfg.player.pos_y = new_y;
}

static void move_left(t_game *game)
{
    float new_x = game->cfg.player.pos_x + game->cfg.player.dir_y * MOVE_SPEED;
    float new_y = game->cfg.player.pos_y - game->cfg.player.dir_x * MOVE_SPEED;

    if (!is_blocked(game, (int)new_x, (int)game->cfg.player.pos_y))
        game->cfg.player.pos_x = new_x;

    if (!is_blocked(game, (int)game->cfg.player.pos_x, (int)new_y))
        game->cfg.player.pos_y = new_y;

}

static void move_right(t_game *game)
{
    float new_x = game->cfg.player.pos_x - game->cfg.player.dir_y * MOVE_SPEED;
    float new_y = game->cfg.player.pos_y + game->cfg.player.dir_x * MOVE_SPEED;

    if (!is_blocked(game, (int)new_x, (int)game->cfg.player.pos_y))
        game->cfg.player.pos_x = new_x;

    if (!is_blocked(game, (int)game->cfg.player.pos_x, (int)new_y))
        game->cfg.player.pos_y = new_y;

}

void player_movement(int key, t_game *game)
{
    if (key == KEY_W)
        move_forward(game);
    else if (key == KEY_S)
        move_backward(game);
    else if (key == KEY_A)
        move_left(game);
    else if (key == KEY_D) 
        move_right(game);
}
