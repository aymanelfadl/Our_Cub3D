/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:41:41 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 15:43:46 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"
#include "bonus.h"

static	int	find_map_width(char **lines, int end)
{
	int	i;
	int	j;
	int	max_with;

	max_with = 0;
	i = 0;
	while (i < end)
	{
		j = 0;
		while (lines[i][j])
		{
			if (lines[i][j] == '\n')
				break ;
			j++;
		}
		if (j > max_with)
			max_with = j;
		i++;
	}
	return (max_with);
}

int	validate_map_chars(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (!is_map_char(map->grid[i][j])
				&& !is_bonus_char(map->grid[i][j]))
			{
				return (ERR_INVALID_CHAR);
			}
			j++;
		}
		i++;
	}
	return (OK);
}

int	find_map_start(char **lines, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (lines[i][j] && ft_isspace(lines[i][j]))
			j++;
		if (lines[i][j] && (lines[i][j] == '1' || lines[i][j] == '0'))
			return (i);
		i++;
	}
	return (-1);
}

static	char	**create_map_grid(int height, int width)
{
	char	**grid;

	int (i), (j);
	grid = malloc(sizeof(char *) * (height + 1));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < height)
	{
		grid[i] = malloc(width + 1);
		if (!grid[i])
		{
			while (i > 0)
				free(grid[--i]);
			free(grid);
			return (NULL);
		}
		j = 0;
		while (j < width)
			grid[i][j++] = ' ';
		grid[i][width] = '\0';
		i++;
	}
	grid[height] = NULL;
	return (grid);
}

int	build_map(t_parser *parser, int start)
{
	int	height;
	int	width;
	int	i;

	height = parser->line_count - start;
	if (height <= 0)
		return (ERR_NO_MAP);
	width = find_map_width(parser->map_lines, parser->line_count);
	parser->map.grid = create_map_grid(height, width);
	if (!parser->map.grid)
		return (ERR_MALLOC);
	parser->map.height = height;
	parser->map.width = width;
	i = 0;
	while (i < height)
	{
		copy_map_line(parser->map.grid[i], parser->map_lines[start + i], width);
		i++;
	}
	return (OK);
}
