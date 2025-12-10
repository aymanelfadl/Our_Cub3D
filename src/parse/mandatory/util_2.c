/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:07:37 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:32:06 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

int	is_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	is_player_char(char c)
{
	return (c == 'E' || c == 'W' || c == 'S' || c == 'N');
}

void	copy_map_line(char *grid, char *line, int width)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n' && i < width)
	{
		grid[i] = line[i];
		i++;
	}
	while (i < width)
		grid[i++] = ' ';
	grid[width] = '\0';
}
