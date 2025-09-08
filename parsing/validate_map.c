/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:14 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:14 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int dfs(t_map map, int y, int x, char **visited)
{
	if (y < 0 || x < 0 || y >= map.height || x >= map.width || map.grid[y][x] == ' ')
		return (0);
	if (map.grid[y][x] == '1' || visited[y][x] == '1')
		return (1);
	visited[y][x] = '1';
	if (!dfs(map, y - 1, x, visited))
		return (0);
	if (!dfs(map, y + 1, x, visited))
		return (0);
	if (!dfs(map, y, x - 1, visited))
		return (0);
	if (!dfs(map, y, x + 1, visited))
		return (0);
	return (1);
}

int is_close_map(t_game *game)
{
	char **visited;
	int result;

	visited = allocate_map_grid(game->cfg.map.height, game->cfg.map.width);
	if (!visited)
		return (perror("Error"), 0);
	result = dfs(game->cfg.map, game->cfg.player.pos_y, game->cfg.player.pos_x,
				 visited);
	ft_free_split(visited);
	if (!result)
		return (0);
	return (1);
}

void check_cell(t_game *game, int y, int x, int *p_found)
{
	char c;

	c = game->cfg.map.grid[y][x];
	if (!is_valid_map_char(c))
		print_err("no part of map IDs");
	if (is_it_player(c))
	{
		if (*p_found)
			print_err("Duplicate Players");
		game->cfg.player.pos_x = x;
		game->cfg.player.pos_y = y;
		game->cfg.player.direction = get_direction(game, c);
		*p_found = 1;
	}
}

int scan_map_cells(t_game *game)
{
	int y;
	int p_found;
	int x;

	y = 0;
	p_found = 0;
	while (y < game->cfg.map.height)
	{
		x = 0;
		while (x < game->cfg.map.width)
		{
			check_cell(game, y, x, &p_found);
			x++;
		}
		y++;
	}
	if (!p_found)
		print_err("no players ?");
	return (1);
}
