#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "parser_internal.h"
#include "bonus.h"

static int	is_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' ' ||
			c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	find_map_width(char **lines, int end)
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
		i++;
	}
	return (max_with);
}

static int	is_player_char(char c)
{
	return (c == 'E' || c == 'W' || c == 'S' || c == 'N');
}

static	void set_player_direction(t_player *player, char dir)
{
	if (dir == 'N')
	{
		player->dir_x = 0.0f;
		player->dir_y = -1.0f;
	}
	else if (dir == 'S')
	{
		player->dir_x = 0.0f;
		player->dir_y = 1.0f;
	}
	else if (dir == 'E')
	{
		player->dir_x = 1.0f;
		player->dir_y = 0.0f;
	}
	else if (dir == 'W')
	{
		player->dir_x = -1.0f;
		player->dir_y = 0.0f;
	}
	player->plane_x = -player->dir_y * 0.66f;
	player->plane_y = player->dir_x * 0.66f;
}
int	find_player(t_parser *parser)
{
	int	i;
	int	j;
	int	count;
	t_map	*map;
	t_player	*player;

	map = &parser->map;
	player = &parser->game->cfg.player;
	count = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (is_player_char(map->grid[i][j]))
			{
				player->pos_x = (float)j + 0.5f;
				player->pos_y = (float)i + 0.5f;
				player->direction = map->grid[i][j];
				
				set_player_direction(player, map->grid[i][j]);
				count++;
			}
			j++;
		}
		i++;
	}
	if (count < 1)
		return (ERR_NO_PLAYER);
	if (count > 1)
		return (ERR_MULTI_PLAYER);
	return (OK);
	
}

int	validate_map_chars(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (!is_map_char(map->grid[i][j]) && !is_bonus_char(map->grid[i][j]))
			{
				return (ERR_INVALID_CHAR);
			}
			j++;
		}
		i++;
	}
	return (OK);
}
int	find_map_start(char **lines, int count)
{
    int i;
    int j;

    i = 0;
    while (i < count)
    {
        j = 0;
        /* skip leading whitespace */
        while (lines[i][j] && ft_isspace(lines[i][j]))
            j++;
        if (lines[i][j] && (lines[i][j] == '1' || lines[i][j] == '0'))
            return (i);
        i++;
    }
    return (-1);
}

static char	**create_map_grid(int height, int width)
{
	char	**grid;
	int		i;
	int		j;

	grid = malloc(sizeof(char *) * (height + 1));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < height)
	{
		grid[i] = malloc(width + 1);
		if (!grid[i])
		{
			while (i > 0)
				free(grid[--i]);
			free(grid);
			return (NULL);
		}
		j = 0;
		while (j < width)
			grid[i][j++] = ' ';
		grid[i][width] = '\0';
		i++;
	}
	grid[height] = NULL;
	return (grid);
}

static	void	copy_map_line(char *grid, char *line, int width)
{
	int i;

	i = 0;
	while (line[i] && line[i] != '\n' && i < width)
	{
		grid[i] = line[i];
		i++;
	}
	while (i < width)
		grid[i++] = ' ';
	grid[width] = '\0';
}

int	build_map(t_parser *parser, int start)
{
	int	height;
	int	width;
	int	i;

	height = parser->line_count - start;
	if (height <= 0)
		return (ERR_NO_MAP);
	width = find_map_width(parser->map_lines, parser->line_count);
	parser->map.grid = create_map_grid(height, width); // empty spaces grid  
	if (!parser->map.grid)
		return (ERR_MALLOC);
	parser->map.height = height;
	parser->map.width = width;
	i = 0;
	while (i < height)
	{
		copy_map_line(parser->map.grid[i], parser->map_lines[start + i], width);
		i++;
	}
	return (OK);
}


