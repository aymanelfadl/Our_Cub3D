/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:41:05 by ykhoussi          #+#    #+#             */
/*   Updated: 2025/12/10 15:41:06 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_bonus.h"

char	*get_bonus_error_msg(int code)
{
	static	char	*bonus_errors[] = {
		[ERR_INVALID_DOOR] = "Door placed in invalid position",
		[ERR_DOOR_TEXTURE] = "Invalid door texture file",
		[ERR_BONUS_CONFIG] = "Invalid bonus configuration"};
	if (code >= ERR_INVALID_DOOR && code <= ERR_BONUS_CONFIG)
		return (bonus_errors[code - ERR_INVALID_DOOR]);
	return (get_error_msg(code));
}
