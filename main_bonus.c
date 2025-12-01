#include "cub3D.h"
#include "parser_bonus.h"

int main(int ac, char *av[])
{
    t_game game;
    t_parser_bonus parser;
    int code;

    if (ac != 2)
    {
        printf("Usage: %s <file.cub>\n", av[0]);
        return (1);
    }

    ft_bzero(&game, sizeof(t_game));
    ft_bzero(&parser, sizeof(t_parser_bonus));
    parser.base.game = &game;

    code = parse_file_bonus(av[1], &parser);
    if (code != OK)
    {
        fprintf(stderr, "Error\nParsing failed with code: %d\n", code);
        fprintf(stderr, "%s\n", get_error_msg(code));
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

    return (0);
}