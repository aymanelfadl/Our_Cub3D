#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

static int	is_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' ' ||
			c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	find_map_width(char **lines, int start, int end)
{
	int	i;
	int	j;
	int	max_with;

	max_with = 0;
	i = 0;
	while (i < end)
	{
		j = 0;
		while (lines[i][j])
		{
			if (lines[i][j] == '\n')
				break;
			j++;	
		}
		if (j > max_with)
			max_with = j;
	}
	return (max_with);
}
	

int	find_map_start(char **lines, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (lines[i][j] && !ft_isspace(lines[i][j]))
			j++;
		if (lines[i][j] && is_map_char(lines[i][j]))
				return (i);
		i++;
	}
	return (-1);
}
static char	**create_map_grid(int height, int width)
{
	// create a 2D array for the map grid
}

int	build_map(t_parser *parser, int start)
{
	int	height;
	int	width;
	int	i;

	height = parser->line_count - start;
	if (height <= 0)
		return (ERR_NO_MAP);
	width = find_map_width(parser->map_lines, start, parser->line_count);
	parser->map.grid = create_map_grid(height, width);
	if (!parser->map.grid)
		return (ERR_MALLOC);
	parser->map.height = height;
	parser->map.width = width;
	// copy map lines into the grid
	return (0);
}
