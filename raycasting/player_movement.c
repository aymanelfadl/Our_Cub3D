/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:57:28 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 21:57:28 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	move_forward(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->cfg.player.pos_x + game->cfg.player.dir_x * MOVE_SPEED;
	new_y = game->cfg.player.pos_y + game->cfg.player.dir_y * MOVE_SPEED;
	apply_movement(game, new_x, new_y);
}

static void	move_backward(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->cfg.player.pos_x - game->cfg.player.dir_x * MOVE_SPEED;
	new_y = game->cfg.player.pos_y - game->cfg.player.dir_y * MOVE_SPEED;
	apply_movement(game, new_x, new_y);
}

static void	move_left(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->cfg.player.pos_x + game->cfg.player.dir_y * MOVE_SPEED;
	new_y = game->cfg.player.pos_y - game->cfg.player.dir_x * MOVE_SPEED;
	apply_movement(game, new_x, new_y);
}

static void	move_right(t_game *game)
{
	float	new_x;
	float	new_y;

	new_x = game->cfg.player.pos_x - game->cfg.player.dir_y * MOVE_SPEED;
	new_y = game->cfg.player.pos_y + game->cfg.player.dir_x * MOVE_SPEED;
	apply_movement(game, new_x, new_y);
}

void	player_movement(int key, t_game *game)
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
