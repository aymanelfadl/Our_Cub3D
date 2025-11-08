#include <math.h>
#include "parser_internal.h"

static int is_allowed_char(char c)
{
    return (c == '0' || c == '1' || c == '2' || c == 'N'
        || c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

static void set_player_direction(t_player *player, char dir)
{
    if (dir == 'N')
    {
        player->dir_x = 0;
        player->dir_y = -1;
        player->direction = NO;
    }
    else if (dir == 'S')
    {
        player->dir_x = 0;
        player->dir_y = 1;
        player->direction = SO;
    }
    else if (dir == 'E')
    {
        player->dir_x = 1;
        player->dir_y = 0;
        player->direction = EA;
    }
    else
    {
        player->dir_x = -1;
        player->dir_y = 0;
        player->direction = WE;
    }
}

int parser_validate_map_chars(const char *line)
{
    int i;

    if (!line)
        return (PARSE_ERR_MAP_INVALID_CHAR);
    i = 0;
    while (line[i])
    {
        if (!is_allowed_char(line[i]))
            return (PARSE_ERR_MAP_INVALID_CHAR);
        i++;
    }
    return (PARSE_OK);
}

void parser_set_player(t_config *cfg, int row, int col, char dir)
{
    t_player    *p;
    double      plane_len;

    p = &cfg->player;
    p->pos_x = (float)col + 0.5f;
    p->pos_y = (float)row + 0.5f;
    set_player_direction(p, dir);
    plane_len = tan((double)FOV_PLANE * M_PI / 360.0);
    p->plane_x = (float)(-p->dir_y * plane_len);
    p->plane_y = (float)(p->dir_x * plane_len);
}

int parser_finalize_player(t_config *cfg, int count)
{
    if (count != 1)
        return (PARSE_ERR_PLAYER_COUNT);
    cfg->have_player = 1;
    return (PARSE_OK);
}
