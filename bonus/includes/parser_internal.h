#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "parser.h"

typedef struct s_parse_state
{
    t_game  *game;
    int     fd;
    char    **map_lines;
    int     map_count;
    int     map_cap;
    int     map_started;
}   t_parse_state;

int     parser_process_file(t_parse_state *state, const char *path);
int     parser_normalize_map(t_parse_state *state);
void    parser_free_map_lines(char **lines, int count);
int     parser_apply_texture(t_config *cfg, int index, const char *value);
int     parser_apply_color(t_config *cfg, const char *value, int is_floor);
int     parser_validate_map_chars(const char *line);
void    parser_set_player(t_config *cfg, int row, int col, char dir);
int     parser_finalize_player(t_config *cfg, int count);
int     parser_validate_enclosure(t_map *map);
void    parser_finalize_resolution(t_config *cfg);

#endif
