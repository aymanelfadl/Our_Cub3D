/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:48:12 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 18:58:14 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "parser_internal.h"

int	load_texture(void *mlx, t_texture *tex)
{
	int	w;
	int	h;

	w = 0;
	h = 0;
	if (!tex || !tex->path)
		return (err_msg("Texture path is NULL", NULL));
	if (!mlx)
		return (err_msg("MLX instance is NULL", tex->path));
	tex->img.mlx_img = mlx_xpm_file_to_image(mlx, tex->path, &w, &h);
	if (!tex->img.mlx_img)
		return (err_msg("Failed to load texture", tex->path));
	tex->img.addr = mlx_get_data_addr(tex->img.mlx_img,
			&tex->img.bpp, &tex->img.line_len, &tex->img.endian);
	tex->width = w;
	tex->height = h;
	tex->img.width = w;
	tex->img.height = h;
	tex->loaded = 1;
	return (0);
}

int	load_door_texture(t_game *game)
{
	t_texture	tex;

	if (game->door_count > 0)
	{
		tex.path = "textures/main_tex/d.xpm";
		if (load_texture(game->mlx, &tex) != 0)
			return (0);
		game->cfg.door_texture = tex;
	}
	return (1);
}

/* Helper to clean up partial loads if hand animation fails */
static int	clean_hand_failure(t_game *game, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		if (game->hand_texture[j].img.mlx_img)
			mlx_destroy_image(game->mlx, game->hand_texture[j].img.mlx_img);
		j++;
	}
	return (0);
}

int	load_hand_texture(t_game *game)
{
	t_texture	tex;
	int			i;
	char		*paths[5];

	paths[0] = "textures/hand/1.xpm";
	paths[1] = "textures/hand/2.xpm";
	paths[2] = "textures/hand/3.xpm";
	paths[3] = "textures/hand/4.xpm";
	paths[4] = "textures/hand/5.xpm";
	i = 0;
	while (i < 5)
	{
		tex.path = paths[i];
		if (load_texture(game->mlx, &tex) != 0)
			return (clean_hand_failure(game, i));
		game->hand_texture[i] = tex;
		i++;
	}
	return (1);
}

int	texture_load_all(void *mlx, t_config *cfg)
{
	int	i;
	int	j;

	if (!cfg)
		return (-1);
	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (cfg->textures[i].path == NULL)
			return (err_msg("Texture path is NULL at index", NULL));
		if (load_texture(mlx, &cfg->textures[i]) != 0)
		{
			j = 0;
			while (j < i)
			{
				if (cfg->textures[j].loaded)
					mlx_destroy_image(mlx, cfg->textures[j].img.mlx_img);
				cfg->textures[j].loaded = 0;
				j++;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}
