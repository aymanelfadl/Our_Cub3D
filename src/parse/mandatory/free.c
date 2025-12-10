/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:05:03 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:46:30 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

void	free_map_grid(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (i < map->height && map->grid[i])
		free(map->grid[i++]);
	free(map->grid);
	map->grid = NULL;
}

void	free_parser_lines(t_parser *parser)
{
	int	i;

	if (!parser->map_lines)
		return ;
	i = 0;
	while (i < parser->line_count)
	{
		free(parser->map_lines[i]);
		i++;
	}
	free(parser->map_lines);
	parser->map_lines = NULL;
}
