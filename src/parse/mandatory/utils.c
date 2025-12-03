#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

char	*skip_spaces(char *str)
{
<<<<<<< HEAD
    return (common_skip_spaces(str));
=======
	while (*str && ft_isspace(*str) )
		str++;
	return (str);
>>>>>>> d99ac4e (refactor(parser): add error handling for invalid characters and update configuration parsing)
}

int is_valid_extension(const char *path)
{
    return (common_is_valid_extension(path));
}

void parser_release_config(t_config *cfg)
{
    common_parser_release_config(cfg);
}
