/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ares <ares@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:03 by aelfadl           #+#    #+#             */
/*   Updated: 2025/11/08 17:30:48 by ares             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "parser.h"

int main(int ac, char *av[])
{
    if (ac != 2)
    {
        printf("Usage: %s <file.cub>\n", av[0]);
        return 1;
    }

    t_game game;
    int     code;

    ft_bzero(&game, sizeof(t_game));
    code = parse_cub_file(av[1], &game);
    if (code != PARSE_OK)
    {
        fprintf(stderr, "Error\n%s\n", parser_strerror(code));
        parser_release_config(&game.cfg);
        return (1);
    }
    if (!start_game(&game))
        return (parser_release_config(&game.cfg), 1);
    parser_release_config(&game.cfg);
    return (0);
}
