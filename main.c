/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:03 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:03 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
