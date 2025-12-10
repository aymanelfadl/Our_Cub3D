/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:34:12 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:34:12 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "parser.h"

int	main(int ac, char *av[])
{
	t_game		game;
	int			code;
	t_parser	parser;

	if (ac != 2)
		return (printf("Usage: %s <file.cub>\n", av[0]), 1);
	ft_bzero(&game, sizeof(t_game));
	ft_bzero(&parser, sizeof(t_parser));
	parser.game = &game;
	code = parse_file(av[1], &parser);
	if (code != OK)
	{
		fprintf(stderr, "Error\n%s\n", get_error_msg(code));
		parser_release_config(&game.cfg);
		return (1);
	}
	if (!start_game(&game))
		return (parser_release_config(&game.cfg), 1);
	parser_release_config(&game.cfg);
	return (0);
}
