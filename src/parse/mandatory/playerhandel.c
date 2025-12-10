/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playerhandel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:11:03 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:32:06 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

void	set_player_direction(t_player *player, char dir)
{
	if (dir == 'N')
	{
		player->dir_x = 0.0f;
		player->dir_y = -1.0f;
	}
	else if (dir == 'S')
	{
		player->dir_x = 0.0f;
		player->dir_y = 1.0f;
	}
	else if (dir == 'E')
	{
		player->dir_x = 1.0f;
		player->dir_y = 0.0f;
	}
	else if (dir == 'W')
	{
		player->dir_x = -1.0f;
		player->dir_y = 0.0f;
	}
	player->plane_x = -player->dir_y * 0.66f;
	player->plane_y = player->dir_x * 0.66f;
}

static	int	check_player_count(int count)
{
	if (count < 1)
		return (ERR_NO_PLAYER);
	if (count > 1)
		return (ERR_MULTI_PLAYER);
	return (OK);
}

static	void	process_player_cell(t_map *map, t_player *player,
								int i, int j)
{
	if (is_player_char(map->grid[i][j]))
	{
		player->pos_x = (float)j + 0.5f;
		player->pos_y = (float)i + 0.5f;
		player->direction = map->grid[i][j];
		set_player_direction(player, map->grid[i][j]);
	}
}

int	find_player(t_parser *parser)
{
	t_map		*map;
	t_player	*player;
	int			i;
	int			j;
	int			count;

	map = &parser->map;
	player = &parser->game->cfg.player;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (is_player_char(map->grid[i][j]))
				count++;
			process_player_cell(map, player, i, j);
		}
	}
	return (check_player_count(count));
}
