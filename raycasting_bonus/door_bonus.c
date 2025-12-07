/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:00:11 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 22:00:11 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_door	*find_door_at(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->door_count)
	{
		if (game->doors[i].map_x == x && game->doors[i].map_y == y)
			return (&game->doors[i]);
		i++;
	}
	return (NULL);
}

int	check_door(t_game *game, float x, float y)
{
	t_door	*door;

	door = find_door_at(game, (int)(x - COLLISION_CONST), (int)(y
				- COLLISION_CONST));
	if (door != NULL && !door->is_open)
		return (1);
	door = find_door_at(game, (int)(x + COLLISION_CONST), (int)(y
				- COLLISION_CONST));
	if (door != NULL && !door->is_open)
		return (1);
	door = find_door_at(game, (int)(x - COLLISION_CONST), (int)(y
				+ COLLISION_CONST));
	if (door != NULL && !door->is_open)
		return (1);
	door = find_door_at(game, (int)(x + COLLISION_CONST), (int)(y
				+ COLLISION_CONST));
	if (door != NULL && !door->is_open)
		return (1);
	return (0);
}

void	toggle_door(t_game *game)
{
	int check_x;
	int check_y;
	t_door *door;

	check_x = (int)(game->cfg.player.pos_x + game->cfg.player.dir_x * 1.5f);
	check_y = (int)(game->cfg.player.pos_y + game->cfg.player.dir_y * 1.5f);
	door = find_door_at(game, check_x, check_y);
	if (door != NULL)
		door->is_open = !door->is_open;
}
