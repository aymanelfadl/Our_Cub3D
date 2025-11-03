#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include "parser_internal.h"

static int is_positive_int(const char *str, int *value)
{
    long    result;
    int     i;

    if (!str || !*str)
        return (0);
    result = 0;
    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        result = result * 10 + (str[i] - '0');
        if (result > INT_MAX)
            return (0);
        i++;
    }
    if (result <= 0)
        return (0);
    *value = (int)result;
    return (1);
}

static char **select_texture_slot(t_config *cfg, int index)
{
    if (index < TEXTURE_COUNT)
        return (&cfg->textures[index].path);
    return (&cfg->sprite_path);
}

static int validate_texture_file(const char *path)
{
    size_t  len;
    int     fd;

    len = ft_strlen(path);
    if (len < 4 || ft_strncmp(path + len - 4, ".xpm", 4))
        return (0);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (0);
    close(fd);
    return (1);
}

int parser_apply_resolution(t_config *cfg, const char *value)
{
    char    **parts;
    int     count;
    int     width;
    int     height;
    int     code;

    if (cfg->have_resolution)
        return (PARSE_ERR_CONFIG_DUP);
    parts = ft_split(value, " \t");
    if (!parts)
        return (PARSE_ERR_ALLOC);
    count = 0;
    while (parts[count])
        count++;
    code = PARSE_OK;
    if (count != 2 || !is_positive_int(parts[0], &width)
        || !is_positive_int(parts[1], &height))
        code = PARSE_ERR_RESOLUTION;
    if (code == PARSE_OK)
    {
        cfg->resolution_width = width;
        cfg->resolution_height = height;
        cfg->have_resolution = 1;
    }
    parser_free_map_lines(parts, count);
    return (code);
}

int parser_apply_texture(t_config *cfg, int index, const char *value)
{
    char    *trim;
    char    **dst;

    dst = select_texture_slot(cfg, index);
    if (*dst)
        return (PARSE_ERR_CONFIG_DUP);
    trim = ft_strtrim(value, " \t");
    if (!trim)
        return (PARSE_ERR_ALLOC);
    if (!*trim || !validate_texture_file(trim))
    {
        free(trim);
        return (PARSE_ERR_TEXTURE);
    }
    *dst = ft_strdup(trim);
    free(trim);
    if (!*dst)
        return (PARSE_ERR_ALLOC);
    if (index < TEXTURE_COUNT)
        cfg->textures[index].id = (t_direction)index;
    else
        cfg->have_sprite = 1;
    return (PARSE_OK);
}
