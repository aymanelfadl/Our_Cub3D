/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:13 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:13 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	*merge_map_colors(char **map)
{
	int		i;
	char	*color_str;
	char	*tmp;

	if (!map || !map[1])
		return (NULL);
	i = 2;
	color_str = ft_strdup(map[1]);
	if (!color_str)
		return (NULL);
	while (map[i])
	{
		tmp = ft_strjoin(color_str, map[i]);
		free(color_str);
		color_str = tmp;
		i++;
	}
	return (color_str);
}

int	parse_color_value(char *str, t_color *color)
{
	char	**split;

	split = ft_split(str, ",");
	if (!split || ft_split_size(split) != 3)
		return (ft_free_split(split), 0);
	color->red = wrap_ft_aoti(split[0]);
	color->green = wrap_ft_aoti(split[1]);
	color->blue = wrap_ft_aoti(split[2]);
	ft_free_split(split);
	return (in_range(color->red, 0, 255) && in_range(color->green, 0, 255)
		&& in_range(color->blue, 0, 255));
}

void	get_color_pointers(t_game *game, int is_floor, int **flag,
		t_color **color)
{
	if (is_floor)
	{
		*flag = &game->cfg.have_floor;
		*color = &game->cfg.floor_color;
	}
	else
	{
		*flag = &game->cfg.have_ceiling;
		*color = &game->cfg.ceiling_color;
	}
}

int	parse_apply_color(t_color *color, char **map)
{
	char	*color_str;

	color_str = merge_map_colors(map);
	if (!color_str || !parse_color_value(color_str, color))
		return (free(color_str), 0);
	free(color_str);
	return (1);
}

int	handle_color(t_game *game, char **map, int is_floor)
{
	int		*flag;
	t_color	*color;

	get_color_pointers(game, is_floor, &flag, &color);
	if (*flag || !parse_apply_color(color, map))
		return (0);
	if (is_floor)
		game->cfg.have_floor = 1;
	else
		game->cfg.have_ceiling = 1;
	return (1);
}
