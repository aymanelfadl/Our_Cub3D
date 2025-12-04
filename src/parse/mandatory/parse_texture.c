#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

static int	is_valid_texture(const char *path)
{
	int fd;
	int	len;

	if (!path)
		return (0);
	len = 0;
	while (path[len])
		len++;
	if (len < 4 || path[len - 4] != '.' || path[len - 3] != 'x' ||
		path[len - 2] != 'p' || path[len - 1] != 'm')
			return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	parse_texture(char *line, char **texture)
{
	char	*path;
	char	*start;
	char	*end;
	char 	**split;

	if (*texture)
		return (ERR_DUPLICATE);
	line = skip_spaces(line);
	split = ft_split(line, " \t\n");
	start = line;
	while (*line && !ft_isspace(*line))
		line++;
	end = line;
	if (start == end)
		return (ERR_INVALID_TEXTURE);
	*end = '\0';
	if (!is_valid_texture(start) || split[1] != NULL)
		return (ERR_INVALID_TEXTURE);
	path = ft_strdup(start);
	if (!path)
		return (ERR_MALLOC);
	*texture = path;
	return (OK);
}
