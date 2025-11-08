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
		print_err("Not a valid extension");
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (-1);
	}
	return (fd);
}

static void	setup_map(t_game *game, const char *file)
{
	int	*dim;

	dim = get_map_dimension(file);
	if (dim)
	{
		game->cfg.map.width = dim[0];
		game->cfg.map.height = dim[1];
		free(dim);
	}
	game->cfg.map.grid = allocate_map_grid(game->cfg.map.height, -1);
	if (!game->cfg.map.grid)
		print_err("Map allocation failed");
}

static void	read_game_file(int fd, t_game *game)
{
	game_info(fd, game);
	close(fd);
}

static void	finalize_game(t_game *game)
{
	normalize_map(game);
	if (!valid_map(game))
		print_err("Invalid map");
}

t_game	*init_game(const char *file)
{
	t_game	*game;
	int		fd;

	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		print_err("Calloc failed");
	fd = open_game_file(file);
	if (fd < 0)
	{
		free(game);
		return (NULL);
	}
	setup_map(game, file);
	read_game_file(fd, game);
	finalize_game(game);
	return (game);
}
