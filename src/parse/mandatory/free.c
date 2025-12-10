/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:05:03 by ykhoussi          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/12/10 16:32:05 by aelfadl          ###   ########.fr       */
=======
/*   Updated: 2025/12/10 18:44:17 by ykhoussi         ###   ########.fr       */
>>>>>>> fdfda8f (fix textures)
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

void	free_map_grid(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (i < map->height && map->grid[i])
		free(map->grid[i++]);
	free(map->grid);
	map->grid = NULL;
}

void	free_parser_lines(t_parser *parser)
{
	int	i;

	if (!parser->map_lines)
		return ;
	i = 0;
	while (i < parser->line_count)
	{
		free(parser->map_lines[i]);
		i++;
	}
	free(parser->map_lines);
	parser->map_lines = NULL;
}

void	texture_free_all(void *mlx, t_config *cfg)
{
	int	i;

	if (!cfg)
		return ;
	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (cfg->textures[i].loaded && cfg->textures[i].img.mlx_img)
		{
			mlx_destroy_image(mlx, cfg->textures[i].img.mlx_img);
			cfg->textures[i].img.mlx_img = NULL;
		}
		cfg->textures[i].loaded = 0;
		i++;
	}
}
