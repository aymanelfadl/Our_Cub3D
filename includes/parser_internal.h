#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "parser.h"
# include "cub3D.h"

typedef struct s_parser
{
    t_game  *game;
    int     fd;
    t_map		map;
    char    **map_lines;
    int     map_count;
    int     map_cap;
    int		line_count;
    int     map_started;
}   t_parser;

int	is_valid_extension(const char *path);
char	*skip_spaces(char *str);
int	parse_texture(char *line, char **texture);
int	find_map_start(char **lines, int count);
int	ft_isspace(char c);
int	build_map(t_parser *parser, int start);
int	validate_map_chars(t_map *map);
int	find_player(t_parser *parser);
int	validate_map_closed(t_map *map);
/* Common/shared implementations (prefix common_) */
char    *common_skip_spaces(char *str);
int     common_is_valid_extension(const char *path);
void    common_parser_release_config(t_config *cfg);
int     common_ft_isspace(char c);
#endif