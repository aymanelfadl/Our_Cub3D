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
    printf("Error\n%s\n", msg);
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

int is_texture(char *identifier)
{
    return (!ft_strcmp(identifier, "NO") ||
            !ft_strcmp(identifier, "SO") ||
            !ft_strcmp(identifier, "WE") ||
            !ft_strcmp(identifier, "EA"));
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
    int size;

    split = ft_split(str, ",");
    if (!split)
        return (0);

    size = ft_split_size(split);
    if (size != 3)
    {
        ft_free_split(split);
        return (0);
    }

    color->red = ft_atoi(split[0]);
    color->green = ft_atoi(split[1]);
    color->blue = ft_atoi(split[2]);

    ft_free_split(split);

    if (!in_range(color->red, 0, 255) ||
        !in_range(color->green, 0, 255) ||
        !in_range(color->blue, 0, 255))
        return (0);

    return (1);
}

int game_config(t_game *game, char **map)
{
    if (ft_split_size(map) != 2)
        return (0);
    if (!ft_strcmp(map[0], "F"))
    {
        if (game->cfg.have_floor == 0)
        {
            if (!parse_color_value(map[1], &game->cfg.floor_color))
                return (0);
            game->cfg.have_floor = 1;
            return (1);
        }
        else
            return (print_err("Duplicate Floor"), 0);
    }
    else if (!ft_strcmp(map[0], "C"))
    {
        if (game->cfg.have_ceiling == 0)
        {
            if (!parse_color_value(map[1], &game->cfg.ceiling_color))
                return (0);
            game->cfg.have_ceiling = 1;
            return (1);
        }
        else
            return (print_err("Duplicate Ceiling"), 0);
    }
    else if (is_texture(map[0]))
    {
        t_texture tex = get_texture(map);

        if (!ft_strcmp(map[0], "NO") && !game->cfg.textures[0].path)
            game->cfg.textures[0] = tex;
        else if (!ft_strcmp(map[0], "SO") && !game->cfg.textures[1].path)
            game->cfg.textures[1] = tex;
        else if (!ft_strcmp(map[0], "WE") && !game->cfg.textures[2].path)
            game->cfg.textures[2] = tex;
        else if (!ft_strcmp(map[0], "EA") && !game->cfg.textures[3].path)
            game->cfg.textures[3] = tex;
        else
            return (print_err("Duplicate direction"), 0);
        return 1;
    }
    else
        return (print_err("wrong identifier"), 0);
}

int *get_map_dimension(const char *file)
{
    int fd;
    char *line;
    int *dimension = malloc(sizeof(int) * 2);
    if (!dimension)
    return NULL;
    
    dimension[0] = 0; // width
    dimension[1] = 0; // height
    
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror("Error\n"), free(dimension), NULL);
    
    int map_started = 0;

    line = get_next_line(fd);
    while (line)
    {
        if (!map_started && (line[0] == '1' || line[0] == '0' || line[0] == ' '))
            map_started = 1;

        if (map_started)
        {
            int len = (int)ft_strlen(line);
            if (dimension[0] < len)
                dimension[0] = len;
            dimension[1]++;
        }

        free(line);
        line = get_next_line(fd);
    }

    close(fd);
    printf("H= %d\n W=%d\n", dimension[1], dimension[0]);
    return dimension;
}

char **create_map(int height, int width)
{
    char **map_grid = ft_calloc(height, sizeof(char *));
    if (!map_grid)
        return NULL;
    int i = 0;
    while(i < height)
    {
        map_grid[i] = ft_calloc(width, sizeof(char));
        if (!map_grid[i])
        {
            while (--i >= 0)
                free(map_grid[i]);
            free(map_grid);
            return NULL;
        }
        i++;
    }
    return map_grid;
}

int map_info(int fd, t_game *game)
{
    char *line;
    int i = 0;
    int map_started = 0;

    game->cfg.map.grid = create_map(game->cfg.map.height, game->cfg.map.width);
    if (!game->cfg.map.grid)
        return (print_err("Map Calloc"), 0);

    line = get_next_line(fd);
    while (line && i < game->cfg.map.height)
    {
        if (!map_started && (line[0] == '1' || line[0] == '0' || line[0] == ' '))
            map_started = 1;
        
        if (map_started)
        {
            int len = (int)ft_strlen(line);
            for (int j = 0; j < game->cfg.map.width; j++)
            {
                if (j < len && line[j] != '\0' && line[j] != '\n')
                    game->cfg.map.grid[i][j] = line[j];
                else
                    game->cfg.map.grid[i][j] = ' ';
            }
            i++;
        }
        free(line);
        line = get_next_line(fd);
    }

    return 1;
}

t_game *game_info(int fd, t_game *game)
{
    char *line;
    int element_index;

    element_index = 0;
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
        if (game_config(game, map))
            element_index++;
        if (element_index == 6)
        {
            ft_free_split(map);
            free(line);
            if (!map_info(fd, game))
                return (print_err("Map"), NULL);
            return (game);
        }
        ft_free_split(map);
        free(line);
        line = get_next_line(fd);
    }
    if (element_index != 6)
        return (print_err("Invalide number of cfg element"), NULL);
    return (game);
}

t_game *init_game(const char *file)
{
    t_game *game;
    int fd;
    int *dimension;

    if (!check_extension(file))
        return (print_err("Not a valid extension"), NULL);

    game = ft_calloc(1, sizeof(t_game));
    if (!game)
        return (print_err("Calloc"), NULL);

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror("Error\n"), NULL);

    dimension = get_map_dimension(file);
    if (dimension)
    {
        game->cfg.map.width = dimension[0];
        game->cfg.map.height = dimension[1];
        free(dimension);
    }

    game_info(fd, game);
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
        printf(" %d: id=%d path=%s\n", i, game->cfg.textures[i].id, game->cfg.textures[i].path);
    if (game->cfg.have_floor)
        printf("Floor color: R=%d G=%d B=%d\n", game->cfg.floor_color.red, game->cfg.floor_color.green, game->cfg.floor_color.blue);
    if (game->cfg.have_ceiling)
        printf("Ceiling color: R=%d G=%d B=%d\n", game->cfg.ceiling_color.red, game->cfg.ceiling_color.green, game->cfg.ceiling_color.blue);
    
    printf("MAP:\n");
    if (game->cfg.map.grid)
    {
        for (int i = 0; i < game->cfg.map.height; i++)
        {
            for (int j = 0; j < game->cfg.map.width; j++)
                printf("%c", game->cfg.map.grid[i][j]);
            printf("\n");
        }
    }

    for (int i = 0; i < TEXTURE_COUNT; i++)
        if (game->cfg.textures[i].path)
            free(game->cfg.textures[i].path);

    if (game->cfg.map.grid)
    {
        for (int i = 0; i < game->cfg.map.height; i++)
            free(game->cfg.map.grid[i]);
        free(game->cfg.map.grid);
    }

    free(game);
    return 0;
}
