#include "parser_internal.h"

static int parse_component(const char *src, int *out)
{
    char    *trim;
    int     i;
    long    value;

    trim = ft_strtrim(src, " \t");
    if (!trim)
        return (0);
    if (!*trim)
    {
        free(trim);
        return (0);
    }
    i = 0;
    value = 0;
    while (trim[i])
    {
        if (!ft_isdigit(trim[i]))
        {
            free(trim);
            return (0);
        }
        value = value * 10 + (trim[i] - '0');
        if (value > 255)
        {
            free(trim);
            return (0);
        }
        i++;
    }
    *out = (int)value;
    free(trim);
    return (1);
}

static int parse_color_parts(const char *value, int rgb[3])
{
    char    **parts;
    int     count;
    int     code;
    int     i;

    parts = ft_split(value, ",");
    if (!parts)
        return (PARSE_ERR_ALLOC);
    count = 0;
    while (parts[count])
        count++;
    code = PARSE_OK;
    if (count != 3)
        code = PARSE_ERR_COLOR;
    i = 0;
    while (code == PARSE_OK && i < 3)
    {
        if (!parse_component(parts[i], &rgb[i]))
            code = PARSE_ERR_COLOR;
        i++;
    }
    parser_free_map_lines(parts, count);
    return (code);
}

int parser_apply_color(t_config *cfg, const char *value, int is_floor)
{
    t_color *color;
    int     *flag;
    int     rgb[3];
    int     code;

    color = &cfg->floor_color;
    flag = &cfg->have_floor;
    if (!is_floor)
    {
        color = &cfg->ceiling_color;
        flag = &cfg->have_ceiling;
    }
    if (*flag)
        return (PARSE_ERR_CONFIG_DUP);
    code = parse_color_parts(value, rgb);
    if (code != PARSE_OK)
        return (code);
    color->red = rgb[0];
    color->green = rgb[1];
    color->blue = rgb[2];
    *flag = 1;
    return (PARSE_OK);
}
