#include <fcntl.h>
#include <unistd.h>
#include "parser_internal.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static int	parse_number(char **str, int *num)
{
	long n;

	n = 0;
	if (!ft_isdigit(**str))
		return (0);
	while (ft_isdigit(**str))
	{
		n = n * 10 + (**str - '0');
		(*str)++;
		if (n > 255)
			return (0);
	}
	*num = (int)n;
	return (1);
}

static int	parse_color(char *line, t_color *color)
{
	int	rgb[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		line = skip_spaces(line);
		if (!parse_number(&line, &rgb[i]))
			return (0);
		line = skip_spaces(line);
		if (i < 2 && *line != ',')
			return (0);
		if (i < 2)
			line++;
		i++;
	}
	line = skip_spaces(line);
	if (*line != '\0' && *line != '\n')
		return (0);
	color->red = rgb[0];
	color->green = rgb[1];
	color->blue = rgb[2];
	return (1);
}
static int	read_file_lines(const char *path, t_parser *parser)
{
	int		fd;
	char	*line;
	char	**new_lines;
	int 	capacity;

	if (!path || !parser)
		return (ERR_ARGS);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ERR_OPEN);
	capacity = 100;
	parser->map_lines = malloc(sizeof(char *) * capacity);
	if (!parser->map_lines)
		return (close(fd), ERR_MALLOC);
	parser->line_count = 0;
	while ((line = get_next_line(fd)))
	{
		if (parser->line_count >= capacity - 1)
		{
			int i;
			
			capacity *= 2;
			new_lines = malloc(sizeof(char *) * capacity);
			if (!new_lines)
				return (close(fd), ERR_MALLOC);
			i = 0;
			while(i < parser->line_count)
			{
				new_lines[i] = parser->map_lines[i];
				i++;
			}
			free(parser->map_lines);
			parser->map_lines = new_lines;
		}
		parser->map_lines[parser->line_count++] = line;
	}
	close(fd);
	return (OK);
}

static int	parse_config_line(char *line, t_config *cfg)
{
	line = skip_spaces(line);
	if (line[0] == 'N' && line[1] == 'O' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->no_texture));
	if (line[0] == 'S' && line[1] == 'O' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->so_texture));
	if (line[0] == 'W' && line[1] == 'E' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->we_texture));
	if (line[0] == 'E' && line[1] == 'A' && ft_isspace(line[2]))
		return (parse_texture(line + 3, &cfg->ea_texture));
	/* Bonus directives: sprite texture (single 'S') and door texture ('DO') */
	if (line[0] == 'S' && ft_isspace(line[1]))
	{
		/* store sprite texture path in config */
		return (parse_texture(line + 1, &cfg->sprite_path));
	}
	if (line[0] == 'D' && line[1] == 'O' && ft_isspace(line[2]))
	{
		return (parse_texture(line + 3, &cfg->door_texture_path));
	}
	if (line[0] == 'F' && ft_isspace(line[1]))
	{
		if (cfg->f_set)
			return (ERR_DUPLICATE);
		if (!parse_color(line + 1, &cfg->floor))
			return (ERR_INVALID_COLOR);
		cfg->f_set = 1;
		return (OK);
	}
	if (line[0] == 'C' && ft_isspace(line[1]))
	{
		if (cfg->c_set)
			return (ERR_DUPLICATE);
		if (!parse_color(line + 1, &cfg->ceiling))
			return (ERR_INVALID_COLOR);
		cfg->c_set = 1;
		return (OK);
	}
	return (-1);
}

int	parse_file(const char *path, t_parser *parser)
{
	int err;
	int i;
	int	map_start;
	t_game	*game_backup;
	char	*line;

	if (!path || !parser)
		return (ERR_ARGS);
	err = is_valid_extension(path);
	if (err != OK)
		return (err);
	game_backup = parser->game;
	ft_memset(parser, 0, sizeof(t_parser));
	parser->game = game_backup;
	err = read_file_lines(path, parser);
	if (err != OK)
		return (err);
	i = 0;
	while (i < parser->line_count){
		line = skip_spaces(parser->map_lines[i]);
		if (*line == '1' || *line == '0')
			break;
		err = parse_config_line(parser->map_lines[i], &parser->game->cfg);
		if (err > 0)
			return (err);
		i++;
	}
	if (!parser->game->cfg.f_set || !parser->game->cfg.c_set ||
		!parser->game->cfg.no_texture || !parser->game->cfg.so_texture ||
		!parser->game->cfg.we_texture || !parser->game->cfg.ea_texture)
		return (ERR_MISSING_CONFIG);
	map_start = find_map_start(parser->map_lines, parser->line_count);
	if (map_start < 0)
		return (ERR_NO_MAP);
	err = build_map(parser, map_start); // after this the parser->map.grid has filled with the map content 
	if (err != OK)
		return (err);
	err = validate_map_chars(&parser->map);
	if (err != OK)
		return (err);
	err = find_player(parser);
	if (err != OK)
		return (err);
	err = validate_map_closed(&parser->map);
	if (err != OK)
		return (err);
	parser->game->cfg.map = parser->map;
	parser->game->cfg.textures[NO].path = parser->game->cfg.no_texture;
	parser->game->cfg.textures[SO].path = parser->game->cfg.so_texture;
	parser->game->cfg.textures[WE].path = parser->game->cfg.we_texture;
	parser->game->cfg.textures[EA].path = parser->game->cfg.ea_texture;
	parser->game->cfg.textures[NO].id = NO;
	parser->game->cfg.textures[SO].id = SO;
	parser->game->cfg.textures[WE].id = WE;
	parser->game->cfg.textures[EA].id = EA;
	return (OK);
}

