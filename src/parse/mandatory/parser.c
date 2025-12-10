/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:52:06 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:52:30 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static	int	validate_config(t_parser *parser)
{
	if (!parser->game->cfg.f_set || !parser->game->cfg.c_set
		|| !parser->game->cfg.no_texture || !parser->game->cfg.so_texture
		|| !parser->game->cfg.we_texture || !parser->game->cfg.ea_texture)
		return (ERR_MISSING_CONFIG);
	return (OK);
}

static	int	validate_and_process_map(t_parser *parser, int map_start)
{
	int	err;

	err = build_map(parser, map_start);
	if (err != OK)
		return (err);
	err = validate_map_chars(&parser->map);
	if (err != OK)
		return (err);
	err = find_player(parser);
	if (err != OK)
		return (err);
	return (validate_map_closed(&parser->map));
}

static	void	setup_textures(t_parser *parser)
{
	parser->game->cfg.textures[NO].path = parser->game->cfg.no_texture;
	parser->game->cfg.textures[SO].path = parser->game->cfg.so_texture;
	parser->game->cfg.textures[WE].path = parser->game->cfg.we_texture;
	parser->game->cfg.textures[EA].path = parser->game->cfg.ea_texture;
	parser->game->cfg.textures[NO].id = NO;
	parser->game->cfg.textures[SO].id = SO;
	parser->game->cfg.textures[WE].id = WE;
	parser->game->cfg.textures[EA].id = EA;
}

static	int	init_parser(t_parser *parser, const char *path)
{
	t_game	*game_backup;
	int		err;

	err = is_valid_extension(path);
	if (err != OK)
		return (err);
	game_backup = parser->game;
	ft_memset(parser, 0, sizeof(t_parser));
	parser->game = game_backup;
	return (read_file_lines(path, parser));
}

int	parse_file(const char *path, t_parser *parser)
{
	int	err;
	int	map_start;

	if (!path || !parser)
		return (ERR_ARGS);
	err = init_parser(parser, path);
	if (err != OK)
		return (err);
	err = parse_config_section(parser);
	if (err != OK)
		return (free_parser_lines(parser), err);
	err = validate_config(parser);
	if (err != OK)
		return (free_parser_lines(parser), err);
	map_start = find_map_start(parser->map_lines, parser->line_count);
	if (map_start < 0)
		return (free_parser_lines(parser), ERR_NO_MAP);
	err = validate_and_process_map(parser, map_start);
	if (err != OK)
		return (free_parser_lines(parser), free_map_grid(&parser->map), err);
	parser->game->cfg.map = parser->map;
	setup_textures(parser);
	free_parser_lines(parser);
	return (OK);
}
