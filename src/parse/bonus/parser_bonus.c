#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"
#include "parser_bonus.h"

int     validate_bonus_chars(t_parser_bonus *parser)
{
	int	i;
	int	j;

	i = 0;
	while (i < parser->base.map.height)
	{
		j = 0;
		while (j < parser->base.map.width)
		{
			if (is_door_char(parser->base.map.grid[i][j]))
			{
				if (validate_door_placement(&parser->base.map, j, i) != OK)
					return (ERR_INVALID_DOOR);
			}
			else if (is_sprite_char(parser->base.map.grid[i][j]))
			{
				if (validate_sprite_placement(&parser->base.map, j, i) != OK)
					return (ERR_INVALID_SPRITE);
			}
			j++;
		}
		i++;
	}
	return (OK);
}

int detect_doors(t_parser_bonus *parser)
{
    int i;
    int j;

    parser->base.game->door_count = 0;
    i = 0;
    while (i < parser->base.map.height)
    {
        j = 0;
		while (j < parser->base.map.width)
        {
            if (is_door_char(parser->base.map.grid[i][j]))
                parser->base.game->door_count++;
            j++;
        }
        i++;
    }
    return (OK);
}

int detect_sprites(t_parser_bonus *parser)
{
    int i;
    int j;

    parser->base.game->sprite_count = 0;
    i = 0;
    while (i < parser->base.map.height)
    {
        j = 0;
		while (j < parser->base.map.width)
        {
            if (is_sprite_char(parser->base.map.grid[i][j]))
                parser->base.game->sprite_count++;
            j++;
        }
        i++;
    }
    return (OK);
}

int     build_bonus_entities(t_parser_bonus *parser)
{
	int	i;
    int	j;
    int	door_idx;
    int	sprite_idx;

	if (parser->base.game->door_count > 0)
	{
		parser->base.game->doors = malloc(sizeof(t_door) * parser->base.game->door_count);
		if (!parser->base.game->doors)
        	return (ERR_MALLOC);
	}
	if (parser->base.game->sprite_count > 0)
	{
		parser->base.game->sprites = malloc(sizeof(t_sprite) * parser->base.game->sprite_count);
		if (!parser->base.game->sprites)
        	return (ERR_MALLOC);
	}
	i = 0;
	door_idx = 0;
	sprite_idx = 0;
	while (i < parser->base.map.height)
	{
		j = 0;
		while (j < parser->base.map.width)
		{
			if (is_door_char(parser->base.map.grid[i][j]))
			{
				parser->base.game->doors[door_idx].map_x = j;
				parser->base.game->doors[door_idx].map_y = i;
				parser->base.game->doors[door_idx].is_open = 0;
    			door_idx++;
			}
			else if (is_sprite_char(parser->base.map.grid[i][j]))
			{
				parser->base.game->sprites[sprite_idx].x = (float)j + 0.5f;
				parser->base.game->sprites[sprite_idx].y = (float)i + 0.5f;
				parser->base.game->sprites[sprite_idx].distance = 0.0f;
                parser->base.game->sprites[sprite_idx].current_frame = 0;
                parser->base.game->sprites[sprite_idx].frame_count = 4;
                parser->base.game->sprites[sprite_idx].frame_timer = 0.0f;
                parser->base.game->sprites[sprite_idx].frame_duration = 0.2f;
				sprite_idx++;
			}
			j++;
		}
		i++;
	}
	return (OK);
}

int parse_file_bonus(const char *path, t_parser_bonus *parser)
{
	int	err;

	if (!path || !parser)
		return (ERR_ARGS);
	err = parse_file(path, &parser->base);
	if (err != OK)
		return (err);
	err = validate_bonus_chars(parser);
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
