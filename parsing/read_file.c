#include "cub3D.h"


int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void print_err(const char *msg)
{
    fprintf(stderr, "Error\n%s\n", msg);
    exit(EXIT_FAILURE);
}

int check_extension(const char *file)
{
    int len = ft_strlen(file);

    if (len < 4)
        return (0);
    if (ft_strcmp(file + len - 4, ".cub") != 0)
        return (0);
    return (1);
}

void ft_free_split(char **arr)
{
    int i;

    if (!arr)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int ft_split_size(char **arr)
{
    int i = 0;

    if (!arr)
        return (0);
    while (arr[i])
        i++;
    return (i);
}

int is_texture(char *identifier, int index)
{
    if (!ft_strcmp(identifier, "NO") && index == 0)
        return (1);
    else if (!ft_strcmp(identifier, "SO") && index == 1)
        return (1);
    else if (!ft_strcmp(identifier, "WE") && index == 2)
        return (1);
    else if (!ft_strcmp(identifier, "EA") && index == 3)
        return (1);
    else
        return (0);
}

void which_identifier(t_texture *texture, char *key)
{
    if (!ft_strcmp(key, "NO"))
        texture->id = NO;
    else if (!ft_strcmp(key, "SO"))
        texture->id = SO;
    else if (!ft_strcmp(key, "WE"))
        texture->id = WE;
    else if (!ft_strcmp(key, "EA"))
        texture->id = EA;
}

t_texture get_texture(char **map)
{
    t_texture texture;

    which_identifier(&texture, map[0]);
    texture.path = ft_strdup(map[1]);
    return texture;
}

int in_range(int n, int min, int max)
{
    return (n >= min && n <= max);
}

int parse_color_value(char *str, t_color *color)
{
    char **split;
    int   size;

    split = ft_split(str, ",");
    if (!split)
        return (-1);

    size = ft_split_size(split);
    if (size != 3)
    {
        ft_free_split(split);
        return (-1);
    }

    color->red   = ft_atoi(split[0]);
    color->green = ft_atoi(split[1]);
    color->blue  = ft_atoi(split[2]);

    ft_free_split(split);

    if (!in_range(color->red, 0, 255) ||
        !in_range(color->green, 0, 255) ||
        !in_range(color->blue, 0, 255))
        return (-1);

    return (0);
}

int game_config(t_game *game, char **map)
{
    static int index = 0;

    if (ft_split_size(map) != 2)
        return 0;

    if (index >= 6)
        return 0;

    if (index < 4)
    {
        if (!is_texture(map[0], index))
            return 0;
        game->cfg.textures[index] = get_texture(map);
    }
    else if (index == 4)
    {
        if (ft_strcmp(map[0], "F") != 0)
            return 0;
        if (parse_color_value(map[1], &game->cfg.floor_color) < 0)
            return 0;
        game->cfg.have_floor = 1;
    }
    else if (index == 5)
    {
        if (ft_strcmp(map[0], "C") != 0)
            return 0;
        if (parse_color_value(map[1], &game->cfg.ceiling_color) < 0)
            return 0;
        game->cfg.have_ceiling = 1;
    }
    index++;
    return 1;
}



t_game *game_info(int fd)
{
    char *line;
    t_game *game;
    
    game = ft_calloc(1, sizeof(t_game));
    if (!game)
        return (print_err("Calloc"), NULL);
    
    line = get_next_line(fd);
    while (line)
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            free(line);
            line = get_next_line(fd);
            continue;
        }
        
        char **map = ft_split(line, " \t\n\v\f\r");
        if (!map)
        {
            free(line);
            line = get_next_line(fd);
            continue;
        }
        if (!game_config(game, map))
            print_err("Invalid file: wrong identifier, order, or color value");
        ft_free_split(map);
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
        return (print_err("Not a valid extension"), NULL);
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror("Error\n"), NULL);
    game = game_info(fd);
    close(fd);
    if (!game)
        return (NULL);
    return game;
}

int main(int ac, char *av[])
{
    if (ac != 2)
    {
        printf("Usage: %s <file.cub>\n", av[0]);
        return 1;
    }

    t_game *game = init_game(av[1]);
    if (!game)
    {
        fprintf(stderr, "Failed to parse config.\n");
        return 1;
    }
    
    printf("Textures:\n");
    for (int i = 0; i < TEXTURE_COUNT; i++)
        printf("  %d: id=%d path=%s\n", i, game->cfg.textures[i].id, game->cfg.textures[i].path);

    if (game->cfg.have_floor)
        printf("Floor color: R=%d G=%d B=%d\n",
               game->cfg.floor_color.red,
               game->cfg.floor_color.green,
               game->cfg.floor_color.blue);

    if (game->cfg.have_ceiling)
        printf("Ceiling color: R=%d G=%d B=%d\n",
               game->cfg.ceiling_color.red,
               game->cfg.ceiling_color.green,
               game->cfg.ceiling_color.blue);

    // Cleanup
    for (int i = 0; i < TEXTURE_COUNT; i++)
        if (game->cfg.textures[i].path)
            free(game->cfg.textures[i].path);
    
    free(game);

    return 0;
}

