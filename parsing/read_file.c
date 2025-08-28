#include "cub3D.h"

void print_err(const char *msg)
{
    printf("Error\n %s\n", msg);
    exit(EXIT_FAILURE);
}

int check_extension(const char *file)
{
    char *extension;

    extension = ft_strrchr(file, '\0');
    if (ft_strncmp((extension - 4), ".cub", 4))
        return (0);
    return (1);
}

t_game *game_info(int fd)
{
    char *line;
    t_game *game;
    int i;

    game = ft_calloc(1, sizeof(t_game));
    if (!game)
        return (print_err("Calloc"), NULL);
    line = get_next_line(fd);
    i = 0;
    while (line)
    {
        char **map = ft_split(line, " \t\n\v\f\r");
        if (!map)
            continue;
        
        free(line);
        line = get_next_line(fd);
    }

    return game;
}


t_game *init_game(const char *file)
{
    t_game *game;
    int fd;

    if (!check_extension(file))
        return (print_err("Not a valide extension"), NULL);
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror("Error\n"), NULL);
    game = game_info(fd);
    if (!game)
        return (NULL);
    return game;
}



int main(int ac, char *av[])
{
    if (ac != 2)
        return (1);
    
    t_game *game = init_game(av[1]);
    (void *)game;
    return 1;
}