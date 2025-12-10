/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:31:10 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 19:01:11 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "cub3D.h"
# include "parser.h"
# include <fcntl.h>
# include <unistd.h>

typedef struct s_parser
{
	t_game	*game;
	int		fd;
	t_map	map;
	char	**map_lines;
	int		map_count;
	int		map_cap;
	int		line_count;
	int		map_started;
}			t_parser;

int			is_valid_extension(const char *path);
char		*skip_spaces(char *str);
int			parse_texture(char *line, char **texture);
int			find_map_start(char **lines, int count);
int			ft_isspace(char c);
int			build_map(t_parser *parser, int start);
int			validate_map_chars(t_map *map);
int			find_player(t_parser *parser);
int			validate_map_closed(t_map *map);
int			parse_color(char *line, t_color *color);
int			parse_config_line(char *line, t_config *cfg);
int			read_file_lines(const char *path, t_parser *parser);
/*free memory*/
void		free_map_grid(t_map *map);
void		free_parser_lines(t_parser *parser);
/*utils       parsing*/
int			is_player_char(char c);
int			is_not_valid(char *str);
void		copy_map_line(char *grid, char *line, int width);
int			parse_config_section(t_parser *parser);
int			is_map_char(char c);
/* Common/shared implementations (prefix common_) */
char		*common_skip_spaces(char *str);
int			common_is_valid_extension(const char *path);
void		common_parser_release_config(t_config *cfg);
int			common_ft_isspace(char c);
int			err_msg(char *msg, char *detail);
#endif
