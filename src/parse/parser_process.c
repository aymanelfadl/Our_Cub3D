#include <fcntl.h>
#include "parser_internal.h"

#define CONFIG_NOT_HANDLED -1

static int is_cub_extension(const char *path)
{
    size_t len;

    if (!path)
        return (0);
    len = ft_strlen(path);
    if (len < 4)
        return (0);
    return (!ft_strncmp(path + len - 4, ".cub", 4));
}

static int open_map_file(t_parse_state *state, const char *path)
{
    if (!is_cub_extension(path))
        return (PARSE_ERR_EXTENSION);
    state->fd = open(path, O_RDONLY);
    if (state->fd < 0)
        return (PARSE_ERR_OPEN_FILE);
    return (PARSE_OK);
}

static int ensure_capacity(t_parse_state *state)
{
    char    **tmp;
    int     i;
    int     new_cap;

    if (state->map_count < state->map_cap)
        return (PARSE_OK);
    new_cap = state->map_cap * 2;
    if (new_cap < 16)
        new_cap = 16;
    tmp = ft_calloc(new_cap, sizeof(char *));
    if (!tmp)
        return (PARSE_ERR_ALLOC);
    i = 0;
    while (i < state->map_count)
    {
        tmp[i] = state->map_lines[i];
        i++;
    }
    free(state->map_lines);
    state->map_lines = tmp;
    state->map_cap = new_cap;
    return (PARSE_OK);
}

static char *duplicate_map_line(const char *line)
{
    size_t  len;
    char    *copy;

    len = ft_strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        len--;
    copy = ft_calloc(len + 1, sizeof(char));
    if (!copy)
        return (NULL);
    ft_memcpy(copy, line, len);
    copy[len] = '\0';
    return (copy);
}

static int append_map_line(t_parse_state *state, char *line)
{
    char    *copy;
    int     code;

    code = ensure_capacity(state);
    if (code != PARSE_OK)
        return (code);
    copy = duplicate_map_line(line);
    if (!copy)
        return (PARSE_ERR_ALLOC);
    code = parser_validate_map_chars(copy);
    if (code != PARSE_OK)
    {
        free(copy);
        return (code);
    }
    state->map_lines[state->map_count] = copy;
    state->map_count += 1;
    state->map_started = 1;
    return (PARSE_OK);
}

static int dispatch_config(t_parse_state *state, const char *key, const char *value)
{
    t_config    *cfg;

    cfg = &state->game->cfg;
    if (key[0] == 'N' && key[1] == 'O' && key[2] == '\0')
        return (parser_apply_texture(cfg, NO, value));
    if (key[0] == 'S' && key[1] == 'O' && key[2] == '\0')
        return (parser_apply_texture(cfg, SO, value));
    if (key[0] == 'W' && key[1] == 'E' && key[2] == '\0')
        return (parser_apply_texture(cfg, WE, value));
    if (key[0] == 'E' && key[1] == 'A' && key[2] == '\0')
        return (parser_apply_texture(cfg, EA, value));
    if (key[0] == 'S' && key[1] == '\0')
        return (parser_apply_texture(cfg, TEXTURE_COUNT, value));
    if (key[0] == 'F' && key[1] == '\0')
        return (parser_apply_color(cfg, value, 1));
    if (key[0] == 'C' && key[1] == '\0')
        return (parser_apply_color(cfg, value, 0));
    return (CONFIG_NOT_HANDLED);
}

static int handle_config_line(t_parse_state *state, char *trim)
{
    char    *value;
    int     i;

    i = 0;
    while (trim[i] && trim[i] != ' ' && trim[i] != '\t')
        i++;
    if (i == 0)
        return (CONFIG_NOT_HANDLED);
    value = trim + i;
    while (*value == ' ' || *value == '\t')
        value++;
    trim[i] = '\0';
    return (dispatch_config(state, trim, value));
}

static int process_line(t_parse_state *state, char *line)
{
    char    *trim;
    int     code;

    trim = ft_strtrim(line, " \t\r\n");
    if (!trim)
        return (PARSE_ERR_ALLOC);
    if (!*trim)
    {
        free(trim);
        if (state->map_started)
            return (PARSE_ERR_MAP_GAP);
        return (PARSE_OK);
    }
    code = handle_config_line(state, trim);
    if (code == CONFIG_NOT_HANDLED)
    {
        free(trim);
        return (append_map_line(state, line));
    }
    free(trim);
    return (code);
}

static int ensure_required_config(t_config *cfg)
{
    int i;

    if (!cfg->have_floor || !cfg->have_ceiling)
        return (PARSE_ERR_CONFIG_MISSING);
    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (!cfg->textures[i].path)
            return (PARSE_ERR_CONFIG_MISSING);
        i++;
    }
    parser_finalize_resolution(cfg);
    return (PARSE_OK);
}

int parser_process_file(t_parse_state *state, const char *path)
{
    char    *line;
    int     code;

    code = open_map_file(state, path);
    if (code != PARSE_OK)
        return (code);
    while (1)
    {
        line = get_next_line(state->fd);
        if (!line)
            break ;
        code = process_line(state, line);
        free(line);
        if (code != PARSE_OK)
            return (code);
    }
    if (!state->map_started)
        return (PARSE_ERR_MAP_MISSING);
    return (ensure_required_config(&state->game->cfg));
}
