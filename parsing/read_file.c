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
	scan_map_cells(game);
	if (!is_close_map(game))
		print_err("MAP should be closed");
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

static void	process_config_line(t_game *game, char *line, int *element_index)
{
	char	**split;

	split = ft_split(line, " \t\n\v\f\r");
	if (!split || ft_split_size(split) == 0)
	{
		ft_free_split(split);
		return ;
	}
	if (game_config(game, split))
		(*element_index)++;
	ft_free_split(split);
}

static void	process_map_line(t_game *game, char *line)
{
	if (!game_map(game, line))
	{
		free(line);
		print_err("something about the map");
	}
}

void	game_info(int fd, t_game *game)
{
	char	*line;
	int		element_index;

	element_index = 0;
	line = get_next_nonempty_line(fd);
	while (line)
	{
		if (element_index == 6)
			process_map_line(game, line);
		else
			process_config_line(game, line, &element_index);
		free(line);
		line = get_next_nonempty_line(fd);
	}
	if (element_index != 6)
		print_err("Invalid cfg element count");
}
