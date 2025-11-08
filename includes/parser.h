#ifndef PARSER_H
# define PARSER_H

# include "cub3D.h"

# define PARSE_OK 0

typedef enum e_parse_error
{
    PARSE_ERR_ARG = 1,
    PARSE_ERR_EXTENSION,
    PARSE_ERR_OPEN_FILE,
    PARSE_ERR_ALLOC,
    PARSE_ERR_CONFIG_DUP,
    PARSE_ERR_CONFIG_MISSING,
    PARSE_ERR_TEXTURE,
    PARSE_ERR_COLOR,
    PARSE_ERR_RESOLUTION,
    PARSE_ERR_MAP_MISSING,
    PARSE_ERR_MAP_INVALID_CHAR,
    PARSE_ERR_MAP_GAP,
    PARSE_ERR_PLAYER_COUNT,
    PARSE_ERR_MAP_OPEN
}   t_parse_error;

int         parse_cub_file(const char *path, t_game *game);
const char  *parser_strerror(int code);
void        parser_release_config(t_config *cfg);

#endif
