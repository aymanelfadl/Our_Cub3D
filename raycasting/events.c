/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:57:28 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:31:19 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->cfg.player.dir_x;
	old_plane_x = game->cfg.player.plane_x;
	game->cfg.player.dir_x = old_dir_x * cos(angle) - game->cfg.player.dir_y
		* sin(angle);
	game->cfg.player.dir_y = old_dir_x * sin(angle) + game->cfg.player.dir_y
		* cos(angle);
	game->cfg.player.plane_x = game->cfg.player.plane_x * cos(angle)
		- game->cfg.player.plane_y * sin(angle);
	game->cfg.player.plane_y = old_plane_x * sin(angle)
		+ game->cfg.player.plane_y * cos(angle);
}

int	close_game(t_game *game)
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

int	handle_key(int key, t_game *game)
{
	player_movement(key, game);
	if (key == 65363)
		rotate_player(game, ROT_SPEED);
	if (key == 65361)
		rotate_player(game, -ROT_SPEED);
	if (key == 65307)
		close_game(game);
	return (0);
}
