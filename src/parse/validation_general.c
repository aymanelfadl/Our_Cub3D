#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

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
#ifdef BONUS
    else
        cfg->have_sprite = 1;
#endif
    return (PARSE_OK);
}
