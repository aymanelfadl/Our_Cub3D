/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:00:11 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 22:00:11 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->cfg.player.dir_x;
	old_plane_x = game->cfg.player.plane_x;
	game->cfg.player.dir_x = old_dir_x * cos(angle) - game->cfg.player.dir_y * sin(angle);
	game->cfg.player.dir_y = old_dir_x * sin(angle) + game->cfg.player.dir_y * cos(angle);
	game->cfg.player.plane_x = game->cfg.player.plane_x * cos(angle)
		- game->cfg.player.plane_y * sin(angle);
	game->cfg.player.plane_y = old_plane_x * sin(angle)
		+ game->cfg.player.plane_y * cos(angle);
}

int	mouse_move(int x, int y, void *game)
{
	t_game		*g;
	int 		delta_x;
	int 		center_x;

	(void)y;
	g = (t_game *)game;
	center_x = WINDOW_WIDTH / 2;
	delta_x = x - center_x;
	 if (delta_x > 0)
        rotate_player(g, ROT_SPEED);
    else if (delta_x < 0)
	{
        rotate_player(g, -ROT_SPEED);
	}
	mlx_mouse_move(g->mlx, g->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	return (0);
}

int	close_game(t_game *game)
{
	if (game && game->mlx)
		texture_free_all(game->mlx, &game->cfg);
	if (game && game->frame.mlx_img)
		mlx_destroy_image(game->mlx, game->frame.mlx_img);
	if (game && game->cfg.door_texture.img.mlx_img)
		mlx_destroy_image(game->mlx, game->cfg.door_texture.img.mlx_img);
	if (game && game->win)
		mlx_destroy_window(game->mlx, game->win);
	parser_release_config(&game->cfg);
	exit(EXIT_SUCCESS);
}

int shoot(int button, int x, int y, void *param)
{
	t_game *g;

	g = (t_game*)param;
	(void)x;
	(void)y;
	
	printf("key %d\n", button);
    
	if (button == 1)
	{
		g->shoot = 1;
		printf("we shoot\n");
	}

	return 0;
}

int	handle_key(int key, t_game *game)
{
	player_movement(key, game);
	if (key == KEY_RIGHT)
		rotate_player(game, ROT_SPEED);
	if (key == KEY_LEFT)
		rotate_player(game, -ROT_SPEED);
	if (key == KEY_E)
		toggle_door(game);
	if (key == 65307)
		close_game(game);
	return (0);
}
