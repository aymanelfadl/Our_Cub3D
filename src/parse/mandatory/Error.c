/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:27:46 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 14:45:58 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static char	*get_config_error(int code)
{
	if (code == OK)
		return ("Success");
	else if (code == ERR_ARGS)
		return ("Invalid arguments");
	else if (code == ERR_EXTENSION)
		return ("File must have .cub extension");
	else if (code == ERR_OPEN)
		return ("Cannot open file");
	else if (code == ERR_MALLOC)
		return ("Memory allocation failed");
	else if (code == ERR_DUPLICATE)
		return ("Duplicate configuration");
	else if (code == ERR_MISSING_CONFIG)
		return ("Missing required configuration");
	else if (code == ERR_INVALID_TEXTURE)
		return ("Invalid texture file");
	else if (code == ERR_INVALID_COLOR)
		return ("Invalid color format (R,G,B 0-255)");
	return (NULL);
}

static char	*get_map_error(int code)
{
	if (code == ERR_NO_MAP)
		return ("Map not found");
	else if (code == ERR_INVALID_CHAR)
		return ("Invalid character in map");
	else if (code == ERR_INVALID_CHARACTHER)
		return ("Invalid character");
	else if (code == ERR_NO_PLAYER)
		return ("No player spawn point");
	else if (code == ERR_MULTI_PLAYER)
		return ("Multiple player spawn points");
	else if (code == ERR_MAP_NOT_CLOSED)
		return ("Map is not closed by walls");
	else if (code == 20)
		return ("Invalid door placement");
	else if (code == 21)
		return ("Invalid sprite placement");
	else if (code == 22)
		return ("Door texture not found or invalid");
	else if (code == 23)
		return ("Sprite texture not found or invalid");
	else if (code == 24)
		return ("Bonus configuration error");
	return (NULL);
}

char	*get_error_msg(int code)
{
	char	*msg;

	msg = get_config_error(code);
	if (msg)
		return (msg);
	msg = get_map_error(code);
	if (msg)
		return (msg);
	return ("Unknown error");
}
