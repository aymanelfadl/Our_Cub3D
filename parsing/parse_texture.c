/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_texture(char *id)
{
	return (!ft_strcmp(id, "NO") || !ft_strcmp(id, "SO") || !ft_strcmp(id, "WE")
		|| !ft_strcmp(id, "EA"));
}

void	which_identifier(t_texture *tex, char *key)
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

t_texture	get_texture(char **map)
{
	t_texture	tex;

	which_identifier(&tex, map[0]);
	if (check_extension(map[1], ".xpm"))
	{
		tex.path = ft_strdup(map[1]);
		tex.img.width = 0;
		tex.img.height = 0;
	}
	else
		tex.path = NULL;
	return (tex);
}

int	handle_texture(t_game *game, char **map)
{
	t_texture	tex;
	int			i;

	if (!is_texture(map[0]) || ft_split_size(map) != 2)
		return (0);
	tex = get_texture(map);
	if (!tex.path)
		return (0);

	if (!ft_strcmp(map[0], "NO"))
		i = 0;
	else if (!ft_strcmp(map[0], "SO"))
		i = 1;
	else if (!ft_strcmp(map[0], "WE"))
		i = 2;
	else
		i = 3;
	
	if (game->cfg.textures[i].path)
	{
    	free(tex.path);
    	return 0;
	}
	
	game->cfg.textures[i] = tex;
	return (1);
}

int	game_config(t_game *game, char **map)
{
	if (!ft_strcmp(map[0], "F"))
		return (handle_color(game, map, 1));
	else if (!ft_strcmp(map[0], "C"))
		return (handle_color(game, map, 0));
	else if (handle_texture(game, map))
		return (1);
	return (0);
}
