/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:48:12 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:32:35 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	load_texture(void *mlx, t_texture *tex)
{
	int	w;
	int	h;

	w = 0;
	h = 0;
	if (!tex || !tex->path)
		return (fprintf(stderr, "Error: texture or path is NULL\n"), -1);
	if (!mlx)
	{
		fprintf(stderr,
			"Error: mlx is NULL when loading texture '%s'\n", tex->path);
		return (-1);
	}
	tex->img.mlx_img = mlx_xpm_file_to_image(mlx, tex->path, &w, &h);
	if (!tex->img.mlx_img)
		return (fprintf(stderr,
				"Error: failed to load texture '%s'\n", tex->path), -1);
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

int	load_hand_texture(t_game *game)
{
	t_texture	tex;
	int			i;
	char		*texture_path[5];

	texture_path[0] = "textures/hand/1.xpm";
	texture_path[1] = "textures/hand/2.xpm";
	texture_path[2] = "textures/hand/3.xpm";
	texture_path[3] = "textures/hand/4.xpm";
	texture_path[4] = "textures/hand/5.xpm";
	i = 0;
	while (i < 5)
	{
		tex.path = texture_path[i];
		if (load_texture(game->mlx, &tex) != 0)
			return (0);
		game->hand_texture[i] = tex;
		i++;
	}
	return (1);
}

int	texture_load_all(void *mlx, t_config *cfg)
{
	int (i), (j);
	if (!cfg)
		return (-1);
	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (cfg->textures[i].path == NULL)
			return (fprintf(stderr,
					"Error: texture path for index %d is NULL\n", i), -1);
		if (load_texture(mlx, &cfg->textures[i]) != 0)
		{
			j = 0;
			while (j < i)
			{
				if (cfg->textures[j].loaded && cfg->textures[j].img.mlx_img)
					mlx_destroy_image(mlx, cfg->textures[j].img.mlx_img);
				cfg->textures[j].loaded = 0;
				++j;
			}
			return (-1);
		}
		++i;
	}
	return (0);
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
		++i;
	}
}
