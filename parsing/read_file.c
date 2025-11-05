/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:14 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:14 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	valid_map(t_game *game)
{
	if (!scan_map_cells(game))
		return (0);
	if (!is_close_map(game))
		return (printf("Error: \nmap must be closed\n"), 0);
	return (1);
}

static char	*get_next_nonempty_line(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line && (line[0] == '\n' || line[0] == '\0'))
	{
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}

static int	process_config_line(t_game *game, char *line, int *element_index)
{
	char	**split;

	split = ft_split(line, " \t\n\v\f\r");
	if (!split || ft_split_size(split) == 0)
		return (ft_free_split(split), 0);
	if (!game_config(game, split))
		return (ft_free_split(split), 0);
	(*element_index)++;
	ft_free_split(split);
	return (1);
}

int	game_info(int fd, t_game *game)
{
	char	*line;
	int		element_index;

	element_index = 0;
	line = get_next_nonempty_line(fd);
	while (line)
	{
		if (element_index == 6)
		{
			if (!game_map(game, line))
				return (free(line), printf("Error: \ninvalid map\n"), 0);
		}
		else
		{
			if (!process_config_line(game, line, &element_index))
				return (free(line), printf("Error: \ninvalid configuration line\n"), 0);
		}
		free(line);
		line = get_next_nonempty_line(fd);
	}
	if (element_index != 6)
		return (printf("Error: \ninvalid configuration element count\n"), 0);
	return (1);
}
