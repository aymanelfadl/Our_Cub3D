/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:44:30 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:31:51 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static	int	is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static	int	is_invalid(char c)
{
	return (c == ' ' || c == '\0');
}

static	int	check_cell_enclosed(t_map *map, int j, int i)
{
	char	up;
	char	down;
	char	left;
	char	right;

	if (j <= 0 || i <= 0 || j >= map->width - 1 || i >= map->height - 1)
		return (ERR_MAP_NOT_CLOSED);
	up = map->grid[i - 1][j];
	down = map->grid[i + 1][j];
	left = map->grid[i][j - 1];
	right = map->grid[i][j + 1];
	if (is_invalid(up) || is_invalid(down)
		|| is_invalid(left) || is_invalid(right))
		return (ERR_MAP_NOT_CLOSED);
	return (OK);
}

int	validate_map_closed(t_map *map)
{
	int	i;
	int	j;
	int	err;

	err = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (is_walkable(map->grid[i][j]))
			{
				err = check_cell_enclosed(map, j, i);
				if (err != OK)
					return (err);
			}
			j++;
		}
		i++;
	}
	return (OK);
}
