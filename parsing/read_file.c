#include "cub3D.h"

void print_err()
{
    printf("Error\n");
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


t_game *init_game(const char *file)
{
    if (!check_extension(file))
        print_err();
    
    return NULL;
}



int main(int ac, char *av[])
{
    if (ac != 2)
        return (1);
    
    t_game *game = init_game(av[1]);
    (void *)game;
    return 1;
}