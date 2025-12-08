/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:57:28 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 21:57:28 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	init_dda(t_game *game, int map_y, int map_x)
{
	if (game->cfg.player.ray.ray_x < 0)
	{
		game->cfg.player.ray.step_x = -1;
		game->cfg.player.ray.distance_x = (game->cfg.player.pos_x - map_x)
			* game->cfg.player.ray.next_cell_x;
	}
	else
	{
		game->cfg.player.ray.step_x = 1;
		game->cfg.player.ray.distance_x = (map_x + 1.0f
				- game->cfg.player.pos_x) * game->cfg.player.ray.next_cell_x;
	}
	
	if (game->cfg.player.ray.ray_y < 0)
	{
		game->cfg.player.ray.step_y = -1;
		game->cfg.player.ray.distance_y = (game->cfg.player.pos_y - map_y)
			* game->cfg.player.ray.next_cell_y;
	}
	else
	{
		game->cfg.player.ray.step_y = 1;
		game->cfg.player.ray.distance_y = (map_y + 1.0f
				- game->cfg.player.pos_y) * game->cfg.player.ray.next_cell_y;
	}
}

static int	check_hit(t_game *game, int map_x, int map_y)
{
	if (game->cfg.map.grid[map_y][map_x] == '1')
		return (1);
	return (0);
}

static void	advance_ray(t_game *game, int *map_x, int *map_y)
{
	if (game->cfg.player.ray.distance_x < game->cfg.player.ray.distance_y)
	{
		game->cfg.player.ray.distance_x += game->cfg.player.ray.next_cell_x;
		*map_x += game->cfg.player.ray.step_x;
		game->cfg.player.ray.hit.side = 0;
	}
	else
	{
		game->cfg.player.ray.distance_y += game->cfg.player.ray.next_cell_y;
		*map_y += game->cfg.player.ray.step_y;
		game->cfg.player.ray.hit.side = 1;
	}
}

static void	perform_dda(t_game *game, int map_x, int map_y)
{
	game->cfg.player.ray.hit.is_hit = 0;
	while (!game->cfg.player.ray.hit.is_hit)
	{
		advance_ray(game, &map_x, &map_y);
		game->cfg.player.ray.hit.is_hit = check_hit(game, map_x, map_y);
		if (game->cfg.player.ray.hit.is_hit)
		{
			game->cfg.player.ray.distance_x -= game->cfg.player.ray.next_cell_x;
			game->cfg.player.ray.distance_y -= game->cfg.player.ray.next_cell_y;
		}
	}
}

int	start_dda(t_game *game)
{
	int	map_x;
	int	map_y;

	map_x = floor(game->cfg.player.pos_x);
	map_y = floor(game->cfg.player.pos_y);
	init_dda(game, map_y, map_x);
	perform_dda(game, map_x, map_y);
	if (!game->cfg.player.ray.hit.side)
		return (WINDOW_HEIGHT / game->cfg.player.ray.distance_x);
	return (WINDOW_HEIGHT / game->cfg.player.ray.distance_y);
}
