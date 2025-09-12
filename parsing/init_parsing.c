/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	open_game_file(const char *file)
{
	int	fd;

	if (!check_extension(file, ".cub"))
		return (-2);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return ( -1);
	return (fd);
}

static int setup_map(t_game *game, const char *file)
{
	int	*dim;

	dim = get_map_dimension(file);
	if (!dim)
		return (0);
	game->cfg.map.width = dim[0];
	game->cfg.map.height = dim[1];
	free(dim);
	game->cfg.map.grid = allocate_map_grid(game->cfg.map.height, -1);
	if (!game->cfg.map.grid)
		return (printf("Error: \nmap allocation failed\n"), 0);
	return (1);
}

static int	finalize_game(t_game *game)
{
	if (normalize_map(game))
	{
		if (valid_map(game))
			return (1);
	}
	return (0);
}

t_game	*init_game(const char *file)
{
	t_game	*game;
	int		fd;

	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (printf("Error: \nmemory allocation failed (calloc)\n"), NULL);
	fd = open_game_file(file);
	if (fd < 0)
	{
		free(game);
		if (fd == -2)
			return (printf("Error: \nmap file must have .cub extension\n"), NULL);
		else
			return (printf("Error: \ncan't open this file\n"), NULL);
	}
	if (!setup_map(game, file))	
		return (ft_free_split(game->cfg.map.grid), free(game), NULL);
	if (!game_info(fd, game) || !finalize_game(game)) 
		return (ft_free_texture(game), ft_free_split(game->cfg.map.grid), free(game), NULL);

	close(fd);
	return (game);
}
