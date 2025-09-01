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

void ft_free_split(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

int ft_split_size(char **arr)
{
    int i = 0;
    if (!arr)
        return 0;
    while (arr[i])
        i++;
    return i;
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
        return(print_err("Texture path"), tex);
    return tex;
}

int in_range(int n, int min, int max)
{
    return n >= min && n <= max;
}

int parse_color_value(char *str, t_color *color)
{
    char **split = ft_split(str, ",");
    if (!split)
        return 0;
    if (ft_split_size(split) != 3)
    {
        ft_free_split(split);
        return 0;
    }

    color->red = ft_atoi(split[0]);
    color->green = ft_atoi(split[1]);
    color->blue = ft_atoi(split[2]);
    ft_free_split(split);

    return in_range(color->red, 0, 255) &&
           in_range(color->green, 0, 255) &&
           in_range(color->blue, 0, 255);
}

int game_config(t_game *game, char **map)
{
    if (ft_split_size(map) != 2)
        return (print_err("Invalid cfg element format"), 0);

    if (!ft_strcmp(map[0], "F"))
    {
        if (!game->cfg.have_floor)
        {
            if (!parse_color_value(map[1], &game->cfg.floor_color))
                return 0;
            game->cfg.have_floor = 1;
            return 1;
        }
        else
            return print_err("Duplicate Floor"), 0;
    }
    else if (!ft_strcmp(map[0], "C"))
    {
        if (!game->cfg.have_ceiling)
        {
            if (!parse_color_value(map[1], &game->cfg.ceiling_color))
                return 0;
            game->cfg.have_ceiling = 1;
            return 1;
        }
        else
            return print_err("Duplicate Ceiling"), 0;
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
        return (print_err("Wrong identifier"), 0);
}


int *get_map_dimension(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("Error");
        return NULL;
    }

    int *dim = malloc(sizeof(int) * 2);
    dim[0] = 0; // width
    dim[1] = 0; // height

    char *line = get_next_line(fd);
    int map_started = 0;
    while (line)
    {
        if (ft_strchr(line, '1') || ft_strchr(line, '0'))
            map_started = 1;
        if (map_started)
        {
            int len = ft_strlen(line);
            if (line[len - 1] == '\n')
                len--;
            if (dim[0] < len)
                dim[0] = len;
            dim[1]++;
        }
        if (map_started && (!ft_strchr(line, '1') || !ft_strchr(line, '0')))
        {
            free(line);
            close(fd);
            print_err("no new lines in map");
            return NULL;
        }
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
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
    }
    return grid;
}
int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' ||
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int game_map(t_game *game, int fd)
{
    char *line;
    int row = 0;
    int player_found = 0;
    int i = 0;
    
    game->cfg.map.grid = allocate_map_grid(game->cfg.map.width, game->cfg.map.height);
    if (!game->cfg.map.grid)
        return 0;

    line = get_next_line(fd);
    while (line && row < game->cfg.map.height)
    {
        int len = ft_strlen(line);

        if (line[len - 1] == '\n')
            len--;

        while (i < len)
        {
            char c = line[i];
            if (!is_valid_map_char(c))
                return (print_err("Invalid character in map"), 0);

            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                if (player_found)
                    return (print_err("Multiple players in map"), 0);

                game->cfg.player.pos_x = i;
                game->cfg.player.pos_y = row;
                player_found = 1;
            }

            game->cfg.map.grid[row][i] = c;
            i++;
        }
        i = len;
        while (i < game->cfg.map.width)
            game->cfg.map.grid[row][i] = 'X';

        free(line);
        row++;
        line = get_next_line(fd);
    }

    if (!player_found)
        print_err("No player found in map");

    return 1;
}

t_game *game_info(int fd, t_game *game)
{
    char *line;
    int element_index = 0;

    while ((line = get_next_line(fd)))
    {
        if (line[0] == '\n' || line[0] == '\0')
        {
            free(line);
            continue;
        }

        char **split = ft_split(line, " \t\n\v\f\r");
        if (!split || ft_split_size(split) == 0)
        {
            ft_free_split(split);
            free(line);
            continue;
        }

        if (element_index < 6)
        {
            if (game_config(game, split))
                element_index++;
        }
        else
        {
            ft_free_split(split);
            free(line);
            break;
        }

        ft_free_split(split);
        free(line);
    }

    if (element_index != 6)
        return (print_err("Invalid cfg element count"), NULL);

    if (!game_map(game, fd))
        return (print_err("Invalid Map"), NULL);

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
