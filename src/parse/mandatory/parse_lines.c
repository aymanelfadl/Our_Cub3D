/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:03:32 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:46:52 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static int	parse_texture_config(char *line, t_config *cfg)
{
	if (line[0] == 'N' && line[1] == 'O' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->no_texture));
	else if (line[0] == 'S' && line[1] == 'O' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->so_texture));
	else if (line[0] == 'W' && line[1] == 'E' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->we_texture));
	else if (line[0] == 'E' && line[1] == 'A' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->ea_texture));
	return (OK);
}

static int	parse_floor_config(char *line, t_config *cfg)
{
	if (cfg->f_set)
		return (ERR_DUPLICATE);
	if (!parse_color(line + 1, &cfg->floor))
		return (ERR_INVALID_COLOR);
	cfg->f_set = 1;
	return (OK);
}

static int	parse_ceiling_config(char *line, t_config *cfg)
{
	if (cfg->c_set)
		return (ERR_DUPLICATE);
	if (!parse_color(line + 1, &cfg->ceiling))
		return (ERR_INVALID_COLOR);
	cfg->c_set = 1;
	return (OK);
}

int	parse_config_line(char *line, t_config *cfg)
{
	line = skip_spaces(line);
	if (line[0] == 'N' || line[0] == 'S' || line[0] == 'W'
		|| line[0] == 'E')
		return (parse_texture_config(line, cfg));
	else if (line[0] == 'F' && ft_isspace(line[1]))
		return (parse_floor_config(line, cfg));
	else if (line[0] == 'C' && ft_isspace(line[1]))
		return (parse_ceiling_config(line, cfg));
	else if (is_not_valid(line))
		return (ERR_INVALID_CHARACTHER);
	return (OK);
}

int	parse_config_section(t_parser *parser)
{
	int		err;
	int		i;
	char	*line;

	i = 0;
	while (i < parser->line_count)
	{
		line = skip_spaces(parser->map_lines[i]);
		if (*line == '1' || *line == '0')
			break ;
		err = parse_config_line(parser->map_lines[i], &parser->game->cfg);
		if (err > 0)
			return (err);
		i++;
	}
	return (OK);
}
