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
			else if (is_sprite_char(map->grid[i][j]))
			{
				if (validate_sprite_placement(map, j, i) != OK)
					return (ERR_INVALID_SPRITE);
			}
			j++;
		}
		i++;
	}
	return (OK);
}

int	detect_doors(t_parser_bonus *parser)
{
    int i;
    int j;

    parser->door_count = 0;
    i = 0;
    while (i < parser->map.height)
    {
        j = 0;
        while (j < parser->map.width)
        {
            if (is_door_char(parser->map.grid[i][j]))
                parser->door_count++;
            j++;
        }
        i++;
    }
    return (OK);
}

int	detect_sprites(t_parser_bonus *parser)
{
    int i;
    int j;

    parser->sprite_count = 0;
    i = 0;
    while (i < parser->map.height)
    {
        j = 0;
        while (j < parser->map.width)
        {
            if (is_sprite_char(parser->map.grid[i][j]))
                parser->sprite_count++;
            j++;
        }
        i++;
    }
    return (OK);
}

int     build_bonus_entities(t_parser *parser)
{
	// Placeholder for building bonus entities like doors and sprites
	// Actual implementation would depend on the game's entity system
	(void)parser; // To avoid unused parameter warning
	return (OK);
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
	err = detect_doors(parser);
	if (err != OK)
		return (err);
	err = detect_sprites(parser);
	if (err != OK)
		return (err);
	err = build_bonus_entities(parser);
	if (err != OK)
		return (err);
	return (OK);
}
