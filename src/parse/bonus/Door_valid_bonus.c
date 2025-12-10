/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Door_valid_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:39:16 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 15:40:13 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include "parser_bonus.h"

int	is_door_char(char c)
{
	return (c == 'D');
}

static	int	is_walkable(char c)
{
	return (c == '0' || is_player_char(c));
}

static	int	is_wall_or_door(char c)
{
	return (c == '1' || c == 'D');
}

int	validate_door_placement(t_map *map, int j, int i)
{
	char	up;
	char	down;
	char	left;
	char	right;

	if (j <= 0 || i <= 0 || j >= map->width - 1 || i >= map->height - 1)
		return (ERR_INVALID_DOOR);
	up = map->grid[i - 1][j];
	down = map->grid[i + 1][j];
	left = map->grid[i][j - 1];
	right = map->grid[i][j + 1];
	if (up == ' ' || down == ' ' || left == ' ' || right == ' ')
		return (ERR_INVALID_DOOR);
	if (is_wall_or_door(up) && is_wall_or_door(down)
		&& is_walkable(left) && is_walkable(right))
		return (OK);
	if (is_wall_or_door(left) && is_wall_or_door(right)
		&& is_walkable(up) && is_walkable(down))
		return (OK);
	return (ERR_INVALID_DOOR);
}
