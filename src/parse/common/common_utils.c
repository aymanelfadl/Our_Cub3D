/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:06:58 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:31:56 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

int	common_ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

char	*common_skip_spaces(char *str)
{
	while (*str && common_ft_isspace(*str))
		str++;
	return (str);
}

int	common_is_valid_extension(const char *path)
{
	int	len;

	if (!path)
		return (ERR_ARGS);
	len = 0;
	while (path[len])
		len++;
	if (len < 4)
		return (ERR_EXTENSION);
	if (path[len - 4] == '.' && path[len - 3] == 'c'
		&& path[len - 2] == 'u' && path[len - 1] == 'b')
		return (OK);
	return (ERR_EXTENSION);
}

void	common_parser_release_config(t_config *cfg)
{
	int	i;

	if (!cfg)
		return ;
	if (cfg->no_texture)
		free(cfg->no_texture);
	if (cfg->so_texture)
		free(cfg->so_texture);
	if (cfg->we_texture)
		free(cfg->we_texture);
	if (cfg->ea_texture)
		free(cfg->ea_texture);
	if (cfg->map.grid)
	{
		i = 0;
		while (i < cfg->map.height && cfg->map.grid[i])
		{
			free(cfg->map.grid[i]);
			i++;
		}
		free(cfg->map.grid);
	}
	ft_bzero(cfg, sizeof(t_config));
}
