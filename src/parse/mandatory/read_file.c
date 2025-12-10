/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:19:41 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:58:44 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static	int	copy_lines_to_new_array(t_parser *parser, char **new_lines)
{
	int	i;

	i = -1;
	while (++i < parser->line_count)
		new_lines[i] = parser->map_lines[i];
	free(parser->map_lines);
	parser->map_lines = new_lines;
	return (OK);
}

static	int	resize_lines_array(t_parser *parser, int *capacity)
{
	char	**new_lines;

	*capacity *= 2;
	new_lines = malloc(sizeof(char *) * (*capacity));
	if (!new_lines)
		return (ERR_MALLOC);
	return (copy_lines_to_new_array(parser, new_lines));
}

static	int	add_line_to_array(t_parser *parser, int *capacity, char *line)
{
	int	status;

	if (parser->line_count >= *capacity - 1)
	{
		status = resize_lines_array(parser, capacity);
		if (status != OK)
			return (status);
	}
	parser->map_lines[parser->line_count++] = line;
	return (OK);
}

int	read_file_lines(const char *path, t_parser *parser)
{
	int		fd;
	char	*line;
	int		capacity;
	int		status;

	if (!path || !parser)
		return (ERR_ARGS);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ERR_OPEN);
	capacity = 100;
	parser->map_lines = malloc(sizeof(char *) * capacity);
	if (!parser->map_lines)
		return (close(fd), ERR_MALLOC);
	parser->line_count = 0;
	line = get_next_line(fd);
	while (line)
	{
		status = add_line_to_array(parser, &capacity, line);
		if (status != OK)
			return (close(fd), ERR_MALLOC);
		line = get_next_line(fd);
	}
	close(fd);
	return (OK);
}
