/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:08:40 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 15:17:16 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include "parser_bonus.h"

int	validate_bonus_chars(t_parser_bonus *parser)
{
	int	i;
	int	j;

	i = 0;
	while (i < parser->base.map.height)
	{
		j = 0;
		while (j < parser->base.map.width)
		{
			if (is_door_char(parser->base.map.grid[i][j]))
			{
				if (validate_door_placement(&parser->base.map, j, i) != OK)
					return (ERR_INVALID_DOOR);
			}
			j++;
		}
		i++;
	}
	return (OK);
}

int	detect_doors(t_parser_bonus *parser)
{
	int	i;
	int	j;

	parser->base.game->door_count = 0;
	i = 0;
	while (i < parser->base.map.height)
	{
		j = 0;
		while (j < parser->base.map.width)
		{
			if (is_door_char(parser->base.map.grid[i][j]))
				parser->base.game->door_count++;
			j++;
		}
		i++;
	}
	return (OK);
}

int	build_bonus_door(t_parser_bonus *parser)
{
	int (i), (j), (d_idx);
	if (parser->base.game->door_count <= 0)
		return (OK);
	parser->base.game->doors = malloc(sizeof(t_door)
			* parser->base.game->door_count);
	if (!parser->base.game->doors)
		return (ERR_MALLOC);
	i = -1;
	d_idx = 0;
	while (++i < parser->base.map.height)
	{
		j = -1;
		while (++j < parser->base.map.width)
		{
			if (is_door_char(parser->base.map.grid[i][j]))
			{
				parser->base.game->doors[d_idx].map_x = j;
				parser->base.game->doors[d_idx].map_y = i;
				parser->base.game->doors[d_idx++].is_open = 0;
			}
		}
	}
	return (OK);
}

int	parse_file_bonus(const char *path, t_parser_bonus *parser)
{
	int	err;

	if (!path || !parser)
		return (ERR_ARGS);
	err = parse_file(path, &parser->base);
	if (err != OK)
		return (err);
	err = validate_bonus_chars(parser);
	if (err != OK)
		return (err);
	err = detect_doors(parser);
	if (err != OK)
		return (err);
	err = build_bonus_door(parser);
	if (err != OK)
		return (err);
	return (OK);
}
