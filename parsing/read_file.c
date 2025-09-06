#include "cub3D.h"



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

int check_extension(const char *file, const char *suffix)
{
    int len = ft_strlen(file);
    if (len < (int)ft_strlen(suffix))
        return 0;
    return (ft_strcmp(file + len - ft_strlen(suffix), suffix) == 0);
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


int game_map(t_game *game, char *line)
{
    static int y;

    if (ft_strchr(line, '1') || ft_strchr(line, '0'))
    {
        game->cfg.map.grid[y] = ft_strdup(line);
        y++;
    }
    return 1;
}

void normalize_map(t_game *game)
{
    char **new_grid;
    int y, x, map_started;

    new_grid = allocate_map_grid(game->cfg.map.height, game->cfg.map.width);
    if (!new_grid)
        print_err("Failed to allocate normalized map");

    y = 0;
    while (y < game->cfg.map.height)
    {
        x = 0;
        map_started = 0;
        while (x < game->cfg.map.width)
        {
            if (x < (int)ft_strlen(game->cfg.map.grid[y]) && game->cfg.map.grid[y][x] != '\n' && game->cfg.map.grid[y][x] != '\0')
            {
                if (game->cfg.map.grid[y][x] == '1' || game->cfg.map.grid[y][x] == '0')
                    map_started = 1;
                
                if (game->cfg.map.grid[y][x] == ' ' && !map_started)
                    new_grid[y][x] = ' ';
                else if (game->cfg.map.grid[y][x] == ' ' && map_started)
                    new_grid[y][x] = '0';
                else
                    new_grid[y][x] = game->cfg.map.grid[y][x];
            }
            else
                new_grid[y][x] = ' ';
            x++;
        }
        new_grid[y][x] = '\0';
        y++;
    }

    ft_free_split(game->cfg.map.grid);
    game->cfg.map.grid = new_grid;
}

int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' ||
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int is_it_player(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

t_direction get_direction(char c)
{
    if (!ft_strcmp("N", &c))
        return NO;
    else if (!ft_strcmp("S", &c))
        return SO;
    else if (!ft_strcmp("E", &c))
        return EA;
    return WE;
}

int dfs(t_map map, int y, int x, char **visited)
{
    if (y < 0 || x < 0 || y >= map.height || x >= map.width || map.grid[y][x] == ' ' )
        return 0;

    if (map.grid[y][x] == '1' || visited[y][x] == '1')
        return 1;
 
    visited[y][x] = '1';

    if (!dfs(map, y - 1, x, visited))
        return 0;
    if (!dfs(map, y + 1, x, visited))
        return 0;
    if (!dfs(map, y, x - 1, visited))
        return 0;
    if (!dfs(map, y, x + 1, visited))
        return 0;

    return 1;
}

int is_close_map(t_game *game)
{
    char **visited = allocate_map_grid(game->cfg.map.height, game->cfg.map.width);
    if (!visited)
        return perror("Error"), 0;

    if (!dfs(game->cfg.map, game->cfg.player.pos_y, game->cfg.player.pos_x, visited))
    {
        ft_free_split(visited);
        return 0;
    }
    ft_free_split(visited);
    return 1;
}

int valid_map(t_game *game)
{
    int y = 0;
    int x;
    int p_found = 0;

    while (y < game->cfg.map.height)
    {
        x = 0;
        while (x < game->cfg.map.width)
        {
            if (is_valid_map_char(game->cfg.map.grid[y][x]))
            {
                if (is_it_player(game->cfg.map.grid[y][x]))
                {
                    if (p_found)
                        print_err("Duplicate Players");
                    game->cfg.player.pos_x = x;
                    game->cfg.player.pos_y = y;
                    game->cfg.player.direction = get_direction(game->cfg.map.grid[y][x]);
                    p_found = 1;
                }
            }
            else
                print_err("no part of map IDs");
            x++;
        }
        y++;
    }
    if (!p_found)
        print_err("no players ?");
    if (!is_close_map(game))
        print_err("MAP  should be closed");
    return 1;
}

void game_info(int fd, t_game *game)
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
        if (element_index == 6)
        {
            if (!game_map(game, line))
            {
                free(line);
                print_err("something about the map");
            }
            free(line);
            line = get_next_line(fd);
            continue;
        }
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
        print_err("Invalid cfg element count");
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
    game->cfg.map.grid = allocate_map_grid(game->cfg.map.height, -1);
    if (!game->cfg.map.grid)
        print_err("Map :Calloc failed");

    printf("h==> %d\nw==> %d\n", game->cfg.map.height, game->cfg.map.width);
    game_info(fd, game);
    if (!game)
        return NULL;
    close(fd);

    normalize_map(game);
    if (!valid_map(game))
        return NULL;

    return game;
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    if (x < 0 || x >= WINDOW_WIDTH|| y < 0 || y >= WINDOW_HEIGHT)
        return;
    char *pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

void init_frame(t_game *game)
{
    game->frame.mlx_img = mlx_new_image (game->mlx, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    game->frame.addr =  mlx_get_data_addr(game->frame.mlx_img, &game->frame.bpp, &game->frame.line_len, &game->frame.endian);
}

int get_scale(int map_w, int map_h, int full_map)
{
    int scale_x, scale_y, scale;
    int max_w, max_h;

    if (full_map)
    {
        scale_x = WINDOW_WIDTH / map_w;
        scale_y = WINDOW_HEIGHT / map_h;
        
        if (scale_x < scale_y)
            scale = scale_x;
        else
            scale = scale_y;

        printf("map scale is :: %d\n", scale); 
    }
    else
    {
        max_w = WINDOW_WIDTH / 4;
        max_h = WINDOW_HEIGHT / 4;
        scale_x = max_w / map_w;
        scale_y = max_h / map_h;
        
        if (scale_x < scale_y)
            scale = scale_x;
        else
            scale = scale_y;
        
        if (scale < 2)
            scale = 2; 
        
        printf("mini map scale is :: %d\n", scale); 
    }

    return scale;
}

void draw_tile(t_game *game, int pixel_x, int pixel_y, int scale, int color)
{
    int y = 0, x;
    while (y < scale)
    {
        x = 0;
        while (x < scale)
        {
            my_mlx_pixel_put(&game->frame, pixel_x + x, pixel_y + y, color);
            x++;
        }
        y++;
    }
}


void render(t_game *game)
{
    int scale = get_scale(game->cfg.map.width, game->cfg.map.height, 0);

    int y = 0, x = 0;

    while (y < game->cfg.map.height)
    {
        x = 0;
        while (x < game->cfg.map.width)
        {
            char cell = game->cfg.map.grid[y][x];
            int color;

            if (is_it_player(cell))
                color = 0xFF0000;
            else if (cell == '0') 
                color = 0xFFFFFF;
            else
                color = 0x00FF00;
            
            int p_x = x * scale;
            int p_y = y * scale;
            draw_tile(game, p_x, p_y, scale, color);
            x++;   
        }
        y++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
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
    }

    printf("PLAYER:\n");
    printf(" Position: x=%d, y=%d\n", game->cfg.player.pos_x, game->cfg.player.pos_y);

    if (game->cfg.player.direction == NO)
        printf(" Direction: North\n");
    else if (game->cfg.player.direction == SO)
        printf(" Direction: South\n");
    else if (game->cfg.player.direction == WE)
        printf(" Direction: West\n");
    else if (game->cfg.player.direction == EA)
        printf(" Direction: East\n");
    else
        printf(" Direction: Unknown (%d)\n", game->cfg.player.direction);

    // =================================================================== //

    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    init_frame(game);
    render(game);
    
    mlx_loop(game->mlx);
    mlx_destroy_window(game->mlx, game->win);
    mlx_destroy_display(game->mlx);

    for (int i = 0; i < game->cfg.map.height; i++)
            if (game->cfg.map.grid[i])
                free(game->cfg.map.grid[i]);

    for (int i = 0; i < TEXTURE_COUNT; i++)
        if (game->cfg.textures[i].path)
            free(game->cfg.textures[i].path);

    free(game->cfg.map.grid);
    free(game);

    return 0;
}
