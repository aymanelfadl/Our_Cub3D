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

void print_info(t_game *game)
{
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
    printf(" Position: x=%.2f, y=%.2f\n", game->cfg.player.pos_x, game->cfg.player.pos_y);

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
    printf("===================\n\n");
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
        exit(EXIT_FAILURE);
    }
    print_info(game);

    // =================================================================== //
    
    if (!start_game(game))
    {
        end_game(game);
        exit(EXIT_FAILURE);
    }
    
    // =================================================================== //
    
    end_game(game);

    return 0;
}
