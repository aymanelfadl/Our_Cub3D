#include "parser_internal.h"

static int print_usage(void)
{
    write(STDERR_FILENO, "Usage: parser_tester <file.cub>\n", 32);
    return (1);
}

int main(int argc, char **argv)
{
    t_game  game;
    t_parser parser;
    int     code;
    char    *error_msg;

    if (argc != 2)
        return (print_usage());
    ft_bzero(&game, sizeof(t_game));
    ft_bzero(&parser, sizeof(t_parser));
    parser.game = &game;
    code = parse_file(argv[1], &parser);
    if (code != OK)
    {
        parser_release_config(&game.cfg);
        ft_putendl_fd("Error", STDERR_FILENO);
        error_msg = get_error_msg(code);
        write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
        write(STDERR_FILENO, "\n", 1);
        return (code);
    }
    parser_release_config(&game.cfg);
    return (0);
}