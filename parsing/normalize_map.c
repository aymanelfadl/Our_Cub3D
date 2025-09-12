/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static char	normalize_cell(t_game *game, int y, int x, int *map_started)
{
	if (x < (int)ft_strlen(game->cfg.map.grid[y])
		&& game->cfg.map.grid[y][x] != '\n' && game->cfg.map.grid[y][x] != '\0')
	{
		if (game->cfg.map.grid[y][x] == '1' || game->cfg.map.grid[y][x] == '0')
			*map_started = 1;
		if (game->cfg.map.grid[y][x] == ' ' && !*map_started)
			return (' ');
		else if (game->cfg.map.grid[y][x] == ' ' && *map_started)
			return ('0');
		else
			return (game->cfg.map.grid[y][x]);
	}
	return (' ');
}

int	normalize_map(t_game *game)
{
	char	**new_grid;
	int		map_started;
	int		y;
	int		x;

	new_grid = allocate_map_grid(game->cfg.map.height, game->cfg.map.width);
	if (!new_grid)
		return (printf("Error: \nfailed to allocate normalized map\n"), 0);
	y = 0;
	while (y < game->cfg.map.height)
	{
		map_started = 0;
		x = 0;
		while (x < game->cfg.map.width)
		{
			new_grid[y][x] = normalize_cell(game, y, x, &map_started);
			x++;
		}
		new_grid[y][game->cfg.map.width] = '\0';
		y++;
	}
	ft_free_split(game->cfg.map.grid);
	game->cfg.map.grid = new_grid;
	return (1);
}
