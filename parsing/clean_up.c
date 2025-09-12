/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void ft_free_texture(t_game *game)
{
    int i;

    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (game->cfg.textures[i].path)
            free(game->cfg.textures[i].path);
        i++;
    }
}

void ft_free_mlx(t_game *game)
{
    if (!game || !game->mlx)
        return;
 
    if (game->frame.mlx_img)
    {
        mlx_destroy_image(game->mlx, game->frame.mlx_img);
        game->frame.mlx_img = NULL;
    }
 
    if (game->win)
    {
        mlx_destroy_window(game->mlx, game->win);
        game->win = NULL;
    }
	mlx_destroy_display(game->mlx);
	free(game->mlx);
}

void end_game(t_game *game)
{
	ft_free_mlx(game);
    ft_free_texture(game);
    ft_free_split(game->cfg.map.grid);
    free(game);
}
