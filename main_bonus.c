#include "cub3D.h"
#include "parser_bonus.h"

void print_spite(t_sprite s)
{
        printf ("s(%.2f, %.2f)\n distance: %.2f\ncurrent frame: %d\nframe count: %d\nframe timer: %.2f\nframe duration: %.2f\n", 
                s.x, s.y, s.distance, s.current_frame, s.frame_count, s.frame_timer, s.frame_duration);
}


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
    int i = game.sprite_count;
    while (--i >= 0)
    {
        printf("id %d\n", i);
        print_spite(game.sprites[i]);
    }
    if (!start_game(&game))
    {
        parser_release_config(&game.cfg);
        return (1);
    }
    
    return (0);
}