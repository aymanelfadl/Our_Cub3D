#include "parser.h"

static int print_usage(void)
{
    write(STDERR_FILENO, "Usage: parser_tester <file.cub>\n", 32);
    return (1);
}

int main(int argc, char **argv)
{
    t_game  game;
    int     code;

    if (argc != 2)
        return (print_usage());
    ft_bzero(&game, sizeof(t_game));
    code = parse_cub_file(argv[1], &game);
    if (code != PARSE_OK)
    {
        parser_release_config(&game.cfg);
        ft_putendl_fd("Error", STDERR_FILENO);
        write(STDERR_FILENO, parser_strerror(code),
            ft_strlen(parser_strerror(code)));
        write(STDERR_FILENO, "\n", 1);
        return (code);
    }
    parser_release_config(&game.cfg);
    return (0);
}
