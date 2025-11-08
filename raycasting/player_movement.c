#include "cub3D.h"

static void move_forward(t_game *game)
{
    float x_move;
    float y_move;

    x_move = game->cfg.player.pos_x + game->cfg.player.dir_x * MOVE_SPEED;
    y_move = game->cfg.player.pos_y + game->cfg.player.dir_y * MOVE_SPEED;

    if (game->cfg.map.grid[(int)game->cfg.player.pos_y][(int)x_move] != '1')
        game->cfg.player.pos_x = x_move;
    if (game->cfg.map.grid[(int)y_move][(int)game->cfg.player.pos_x] != '1')
        game->cfg.player.pos_y = y_move;
}

static void move_backward(t_game *game)
{
    float x_move;
    float y_move;

    x_move = game->cfg.player.pos_x - game->cfg.player.dir_x * MOVE_SPEED;
    y_move = game->cfg.player.pos_y - game->cfg.player.dir_y * MOVE_SPEED;

    if (game->cfg.map.grid[(int)game->cfg.player.pos_y][(int)x_move] != '1')
        game->cfg.player.pos_x = x_move;
    if (game->cfg.map.grid[(int)y_move][(int)game->cfg.player.pos_x] != '1')
        game->cfg.player.pos_y = y_move;
}

static void move_left(t_game *game)
{
    float x_move;
    float y_move;

    x_move = game->cfg.player.pos_x - game->cfg.player.plane_x * MOVE_SPEED;
    y_move = game->cfg.player.pos_y - game->cfg.player.plane_y * MOVE_SPEED;

    if (game->cfg.map.grid[(int)game->cfg.player.pos_y][(int)x_move] != '1')
        game->cfg.player.pos_x = x_move;
    if (game->cfg.map.grid[(int)y_move][(int)game->cfg.player.pos_x] != '1')
        game->cfg.player.pos_y = y_move;
}

static void move_right(t_game *game)
{
    float x_move;
    float y_move;
    x_move = game->cfg.player.pos_x + game->cfg.player.plane_x * MOVE_SPEED;
    y_move = game->cfg.player.pos_y + game->cfg.player.plane_y * MOVE_SPEED;

    if (game->cfg.map.grid[(int)game->cfg.player.pos_y][(int)x_move] != '1')
        game->cfg.player.pos_x = x_move;
    if (game->cfg.map.grid[(int)y_move][(int)game->cfg.player.pos_x] != '1')
        game->cfg.player.pos_y = y_move;
}

void player_movement(int key, t_game *game)
{
    if (key == 119)
        move_forward(game);
    else if (key == 115)
        move_backward(game);
    else if (key == 97)
        move_left(game);
    else if (key == 100) 
        move_right(game);
}
