/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:07:45 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:32:06 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

char	*skip_spaces(char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

int	is_valid_extension(const char *path)
{
	return (common_is_valid_extension(path));
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	is_not_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] != '\n'))
	{
		if (str[i] != ' ' || str[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

void	parser_release_config(t_config *cfg)
{
	common_parser_release_config(cfg);
}
