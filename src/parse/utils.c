#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

char	*skip_spaces(char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

int is_valid_extension(const char *path)
{
    int len;

    if (!path)
        return (ERR_ARGS);
    len = 0;
    while (path[len])
        len++;
    if (len < 4)
        return (ERR_EXTENSION);  
    if (path[len - 4] == '.' && path[len - 3] == 'c' &&
        path[len - 2] == 'u' && path[len - 1] == 'b')
        return (OK);
    return (ERR_EXTENSION);
}

void parser_release_config(t_config *cfg)
{
    int i;

    if (!cfg)
        return;
    if (cfg->no_texture)
        free(cfg->no_texture);
    if (cfg->so_texture)
        free(cfg->so_texture);
    if (cfg->we_texture)
        free(cfg->we_texture);
    if (cfg->ea_texture)
        free(cfg->ea_texture);
    if (cfg->map.grid)
    {
        i = 0;
        while (i < cfg->map.height && cfg->map.grid[i])
        {
            free(cfg->map.grid[i]);
            i++;
        }
        free(cfg->map.grid);
    }
    ft_bzero(cfg, sizeof(t_config));
}
