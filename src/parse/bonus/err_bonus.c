/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:41:05 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 16:31:51 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_bonus.h"

static char	*get_bonus_error(int code)
{
	if (code == ERR_INVALID_DOOR)
		return ("Door placed in invalid position");
	else if (code == ERR_DOOR_TEXTURE)
		return ("Invalid door texture file");
	else if (code == ERR_BONUS_CONFIG)
		return ("Invalid bonus configuration");
	return (NULL);
}

char	*get_bonus_error_msg(int code)
{
	char	*msg;

	msg = get_bonus_error(code);
	if (msg)
		return (msg);
	return (get_error_msg(code));
}
