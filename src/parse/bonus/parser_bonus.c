#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"
#include "parser_bonus.h"

int     validate_bonus_chars(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (is_door_char(map->grid[i][j]))
			{
				if (validate_door_placement(map, j, i) != OK)
					return (ERR_INVALID_DOOR);
			}
			j++;
		}
		
		i++;
	}
	
}

int	parse_file_bonus(const char *path, t_parser *parser)
{
	int	err;

	if (!path || !parser)
		return (ERR_ARGS);
	err = parse_file(path, parser);
	if (err != OK)
		return (err);
	err = validate_bonus_chars(&parser->map);
	if (err != OK)
		return (err);
}