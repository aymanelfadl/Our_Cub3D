/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 11:11:14 by aelfadl           #+#    #+#             */
/*   Updated: 2025/09/06 11:11:14 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_duplicate(int *flag, int is_floor)
{
	if (*flag)
	{
		if (is_floor)
			print_err("Duplicate Floor");
		else
			print_err("Duplicate Ceiling");
	}
}

int	check_extension(const char *file, const char *suffix)
{
	int	len;

	len = ft_strlen(file);
	if (len < (int)ft_strlen(suffix))
		return (0);
	return (ft_strcmp(file + len - ft_strlen(suffix), suffix) == 0);
}

int	is_config(char *str)
{
	if (ft_strnstr(str, "F", ft_strlen(str)) || ft_strnstr(str, "C",
			ft_strlen(str)) || ft_strnstr(str, "NO", ft_strlen(str))
		|| ft_strnstr(str, "SO", ft_strlen(str)) || ft_strnstr(str, "WE",
			ft_strlen(str)) || ft_strnstr(str, "EA", ft_strlen(str)))
		return (0);
	return (1);
}

int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W');
}

int	is_it_player(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
