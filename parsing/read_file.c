#include "cub3D.h"

int ft_split_size(char **arr)
{
    int i = 0;
    if (!arr)
        return 0;
    while (arr[i])
        i++;
    return i;
}

void debug_print_split(char **split, const char *original_line)
{
    printf("Original line: '%s'\n", original_line);
    printf("Split size: %d\n", ft_split_size(split));

    if (!split)
    {
        printf("Split is NULL\n");
        return;
    }

    for (int i = 0; split[i]; i++)
    {
        printf("split[%d]: '%s' (length: %zu)\n", i, split[i], strlen(split[i]));
    }
    printf("===================\n\n");
}

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

void ft_free_split(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

int check_extension(const char *file, const char *suffix)
{
    int len = ft_strlen(file);
    if (len < (int)ft_strlen(suffix))
        return 0;
    return (ft_strcmp(file + len - ft_strlen(suffix), suffix) == 0);
}

int is_texture(char *id)
{
    return (!ft_strcmp(id, "NO") || !ft_strcmp(id, "SO") ||
            !ft_strcmp(id, "WE") || !ft_strcmp(id, "EA"));
}

void which_identifier(t_texture *tex, char *key)
{
    if (!ft_strcmp(key, "NO"))
        tex->id = NO;
    else if (!ft_strcmp(key, "SO"))
        tex->id = SO;
    else if (!ft_strcmp(key, "WE"))
        tex->id = WE;
    else if (!ft_strcmp(key, "EA"))
        tex->id = EA;
}

t_texture get_texture(char **map)
{
    t_texture tex;
    which_identifier(&tex, map[0]);
    if (check_extension(map[1], ".xpm"))
        tex.path = ft_strdup(map[1]);
    else
        return (print_err("Texture path"), tex);
    return tex;
}

int in_range(int n, int min, int max)
{
    return n >= min && n <= max;
}

int wrap_ft_aoti(const char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return -1;
        i++;
    }
    return (ft_atoi(str));
}

int parse_color_value(char *str, t_color *color)
{
    char **split = ft_split(str, ",");
    if (!split || ft_split_size(split) != 3)
        return 0;

    color->red = wrap_ft_aoti(split[0]);
    color->green = wrap_ft_aoti(split[1]);
    color->blue = wrap_ft_aoti(split[2]);
    ft_free_split(split);

    return in_range(color->red, 0, 255) &&
           in_range(color->green, 0, 255) &&
           in_range(color->blue, 0, 255);
}

int game_config(t_game *game, char **map)
{
    if (!ft_strcmp(map[0], "F"))
    {
        if (!game->cfg.have_floor)
        {
            int i = 2;
            char *color_str = ft_strdup(map[1]);
            while (map[i])
            {
                char *tmp = ft_strjoin(color_str, map[i]);
                free(color_str);
                color_str = tmp;
                i++;
            }
            if (!parse_color_value(color_str, &game->cfg.floor_color))
            {
                free(color_str);
                print_err("invalide color");
            }
            free(color_str);
            game->cfg.have_floor = 1;
            return 1;
        }
        else
            print_err("Duplicate Floor");
    }
    else if (!ft_strcmp(map[0], "C"))
    {
        if (!game->cfg.have_ceiling)
        {
            int i = 2;
            char *color_str = ft_strdup(map[1]);
            while (map[i])
            {
                char *tmp = ft_strjoin(color_str, map[i]);
                free(color_str);
                color_str = tmp;
                i++;
            }
            if (!parse_color_value(color_str, &game->cfg.ceiling_color))
            {
                free(color_str);
                print_err("invalide color");
            }
            free(color_str);
            game->cfg.have_ceiling = 1;
            return 1;
        }
        else
            print_err("Duplicate Ceiling");
    }
    else if (is_texture(map[0]) && ft_split_size(map) == 2)
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
            print_err("Duplicate direction");
        return 1;
    }
    return (print_err("Wrong identifier"), 0);
}

int is_config(char *str)
{
    if (ft_strnstr(str, "F", ft_strlen(str)) ||
        ft_strnstr(str, "C", ft_strlen(str)) ||
        ft_strnstr(str, "NO", ft_strlen(str)) ||
        ft_strnstr(str, "SO", ft_strlen(str)) ||
        ft_strnstr(str, "WE", ft_strlen(str)) ||
        ft_strnstr(str, "EA", ft_strlen(str)))
        return 0;
    return 1;
}


int *get_map_dimension(const char *file)
{
    int fd;
    char *line;
    int *dim;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror("Error"), NULL);
    dim = malloc(2 * sizeof(int));
    if (!dim)
        return (perror("Error"), NULL);

    dim[0] = 0; // width
    dim[1] = 0; // height

    line = get_next_line(fd);
    
    int map_started = 0;

    while (line)
    {
        if (line[0] == '\n' || line[0] == '\0' || !is_config(line))
        {
            free(line);
            if (map_started)
                print_err("the map content which always has to be the last && no new lines in side the map :)");
            line = get_next_line(fd);
            continue;
        }
        if (ft_strchr(line, '1') || ft_strchr(line, '0'))
        {
            map_started = 1;
            if (dim[0] < (int)ft_strlen(line))
            dim[0] = ft_strlen(line);
            dim[1]++;
        }
        free(line);
        line = get_next_line(fd);
    }
    return dim;
}

char **allocate_map_grid(int width, int height)
{
    int i;
    char **grid;

    i = 0;
    grid = ft_calloc(height + 1, sizeof(char *));
    if (!grid)
        return NULL;

    while (i < height)
    {
        grid[i] = ft_calloc(width + 1, sizeof(char));
        if (!grid[i])
        {
            while (--i > -1)
                free(grid[i]);
            free(grid);
            return NULL;
        }
        i++;
    }
    return grid;
}
int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' ||
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

// int game_map(t_game *game, char *line)
// {

//     return 0;
// }

t_game *game_info(int fd, t_game *game)
{
    char *line;
    int element_index = 0;

    line = get_next_line(fd);
    while (line)
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            free(line);
            line = get_next_line(fd);
            continue;
        }
        // if (element_index == 6)
        // {
        //     if (!game_map(game, line))
        //     {
        //         free(line);
        //         print_err("invalid map");
        //     }
        //     free(line);
        //     line = get_next_line(fd);
        //     continue;
        // }
        char **split = ft_split(line, " \t\n\v\f\r");
        if (!split || ft_split_size(split) == 0)
        {
            ft_free_split(split);
            free(line);
            line = get_next_line(fd);
            continue;
        }
        
        if (game_config(game, split))
            element_index++;
        else
        {
            ft_free_split(split);
            free(line);
            break;
        }

        ft_free_split(split);
        free(line);
        line = get_next_line(fd);
    }

    if (element_index != 6)
        return (print_err("Invalid cfg element count"), NULL);

    return game;
}

t_game *init_game(const char *file)
{
    if (!check_extension(file, ".cub"))
        return print_err("Not a valid extension"), NULL;

    t_game *game = ft_calloc(1, sizeof(t_game));
    if (!game)
        return print_err("Calloc failed"), NULL;

    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("Error");
        return NULL;
    }

    int *dim = get_map_dimension(file);
    if (dim)
    {
        game->cfg.map.width = dim[0];
        game->cfg.map.height = dim[1];
        free(dim);
    }
    printf("h==> %d\nw==> %d\n", game->cfg.map.height, game->cfg.map.width);
    game_info(fd, game);
    close(fd);
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
            if (game->cfg.map.grid[i])
                printf("%s\n", game->cfg.map.grid[i]);

        for (int i = 0; i < game->cfg.map.height; i++)
            if (game->cfg.map.grid[i])
                free(game->cfg.map.grid[i]);
    }

    for (int i = 0; i < TEXTURE_COUNT; i++)
        if (game->cfg.textures[i].path)
            free(game->cfg.textures[i].path);
    free(game->cfg.map.grid);
    free(game);

    return 0;
}
