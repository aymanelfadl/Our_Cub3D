/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:03 by aelfadl           #+#    #+#             */
/*   Updated: 2025/11/30 20:17:08 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "parser.h"
#include "parser_internal.h"

int main(int ac, char *av[])
{
    if (ac != 2)
    {
        printf("Usage: %s <file.cub>\n", av[0]);
        return 1;
    }
    t_game game;
    int     code;
    t_parser parser;
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
    game.mlx = mlx_init();
    if (!game.mlx)
    {
        fprintf(stderr, "Error\nFailed to initialize MLX\n");
        parser_release_config(&game.cfg);
        return (1);
    }
    if (texture_load_all(game.mlx, &game.cfg) != 0)
    {
        fprintf(stderr, "Error\nFailed to load textures\n");
        parser_release_config(&game.cfg);
        return (1);
    }
    start_game(&game);
    parser_release_config(&game.cfg);
    return (0);
}
