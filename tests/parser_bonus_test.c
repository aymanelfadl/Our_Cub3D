/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ares <ares@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:50:25 by Yoares            #+#    #+#             */
/*   Updated: 2025/11/30 23:44:03 by ares             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_bonus.h"
#include <stdio.h>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage: %s <file.cub>\n", av[0]);
		return (1);
	}

	t_game game;
	t_parser_bonus parser;
	int code;

	ft_bzero(&game, sizeof(game));
	ft_bzero(&parser, sizeof(parser));
	parser.base.game = &game;

	code = parse_file_bonus(av[1], &parser);
	if (code != OK)
	{
		char *msg = NULL;
		if (code >= ERR_INVALID_DOOR && code <= ERR_BONUS_CONFIG)
			msg = get_bonus_error_msg(code);
		else
			msg = get_error_msg(code);
		fprintf(stderr, "Parse error %d: %s\n", code, msg ? msg : "(no message)");
		parser_release_config(&game.cfg);
		return (1);
	}

	printf("Parse OK\n");
	printf("Map size: %d x %d\n", parser.base.map.width, parser.base.map.height);
	printf("Doors: %d, Sprites: %d\n", game.door_count, game.sprite_count);

	parser_release_config(&game.cfg);
	return (0);
}
