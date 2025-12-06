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
    if (!start_game(&game))
    {
        parser_release_config(&game.cfg);
        return (1);
    }  
    return (0);
}
