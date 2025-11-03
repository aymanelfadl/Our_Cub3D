/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_map_line(const char *line)
{
	return (ft_strchr(line, '1') || ft_strchr(line, '0'));
}

static void	update_dimensions(int *dim, const char *line)
{
	int	len;

	len = ft_strlen(line);
	if (dim[0] < len)
		dim[0] = len;
	dim[1]++;
}

static void	process_line(int *dim, char *line, int *map_started)
{
	if (line[0] == '\n' || line[0] == '\0' || !is_config(line))
	{
		free(line);
		if (*map_started)
			print_err("Map must be last with no new lines inside");
		return ;
	}
	if (is_map_line(line))
	{
		*map_started = 1;
		update_dimensions(dim, line);
	}
	free(line);
}

int	*get_map_dimension(const char *file)
{
	int		fd;
	int		*dim;
	char	*line;
	int		map_started;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror("Error"), NULL);
	dim = malloc(2 * sizeof(int));
	if (!dim)
		return (perror("Error"), NULL);
	dim[0] = 0; // width
	dim[1] = 0; // height
	line = get_next_line(fd);
	map_started = 0;
	while (line)
	{
		process_line(dim, line, &map_started);
		line = get_next_line(fd);
	}
	close(fd);
	return (dim);
}

int	game_map(t_game *game, char *line)
{
	static int	y = 0;

	if (is_map_line(line))
	{
		game->cfg.map.grid[y] = ft_strdup(line);
		y++;
	}
	return (1);
}
