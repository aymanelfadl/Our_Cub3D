/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:59:04 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:01:15 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static	int	parse_number(char **str, int *num)
{
	long	n;

	n = 0;
	if (!ft_isdigit(**str))
		return (0);
	while (ft_isdigit(**str))
	{
		n = n * 10 + (**str - '0');
		(*str)++;
		if (n > 255)
			return (0);
	}
	*num = (int)n;
	return (1);
}

int	parse_color(char *line, t_color *color)
{
	int	rgb[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		line = skip_spaces(line);
		if (!parse_number(&line, &rgb[i]))
			return (0);
		line = skip_spaces(line);
		if (i < 2 && *line != ',')
			return (0);
		if (i < 2)
			line++;
		i++;
	}
	line = skip_spaces(line);
	if (*line != '\0' && *line != '\n')
		return (0);
	color->red = rgb[0];
	color->green = rgb[1];
	color->blue = rgb[2];
	return (1);
}
