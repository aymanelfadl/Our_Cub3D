/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:00:11 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 22:00:11 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_player(char c)
{
	return (c == 'E' || c == 'W' || c == 'S' || c == 'N');
}

static void	draw_player(t_game *game)
{
	int	player_radius;
	int	py;
	int	center_x;
	int	center_y;
	int	px;

	player_radius = 4;
	center_x = RADIUS * TILE_SIZE;
	center_y = RADIUS * TILE_SIZE;
	py = -player_radius;
	while (py <= player_radius)
	{
		px = -player_radius;
		while (px <= player_radius)
		{
			if (px * px + py * py <= player_radius * player_radius)
				my_mlx_pixel_put(&game->minimap, center_x + px, center_y + py,
					0xFF0000);
			px++;
		}
		py++;
	}
}

static int	get_tile_color(t_game *game, int map_x, int map_y)
{
	int	color;

	color = 0x2C2D2D;
	if (map_y >= 0 && map_y < game->cfg.map.height && map_x >= 0
		&& map_x < game->cfg.map.width)
	{
		if (game->cfg.map.grid[map_y][map_x] == '1')
			color = 0x565757;
		else if (game->cfg.map.grid[map_y][map_x] == '0'
			|| is_player(game->cfg.map.grid[map_y][map_x]))
			color = color_to_int(game->cfg.floor);
		else if (game->cfg.map.grid[map_y][map_x] == 'D')
			color = 0x00FF00;
	}
	return (color);
}

static void	draw_tile(t_game *game, int screen_x, int screen_y, int color)
{
	int	tile_y;
	int	tile_x;
	int	px;
	int	py;

	tile_y = 0;
	while (tile_y < TILE_SIZE)
	{
		tile_x = 0;
		while (tile_x < TILE_SIZE)
		{
			px = screen_x * TILE_SIZE + tile_x;
			py = screen_y * TILE_SIZE + tile_y;
			my_mlx_pixel_put(&game->minimap, px, py, color);
			tile_x++;
		}
		tile_y++;
	}
}

void	draw_minimap(t_game *game)
{
	int	screen_y;
	int	screen_x;
	int	map_x;
	int	map_y;

	map_y = (int)game->cfg.player.pos_y - RADIUS;
	screen_y = 0;
	while (screen_y < RADIUS * 2)
	{
		map_x = (int)game->cfg.player.pos_x - RADIUS;
		screen_x = 0;
		while (screen_x < RADIUS * 2)
		{
			draw_tile(game, screen_x, screen_y, get_tile_color(game, map_x,
					map_y));
			map_x++;
			screen_x++;
		}
		map_y++;
		screen_y++;
	}
	draw_player(game);
}
