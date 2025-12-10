/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:31:10 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:31:10 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "cub3D.h"
# include "parser_internal.h"

typedef struct s_parser	t_parser;

typedef enum e_parse_error
{
	OK = 0,
	ERR_ARGS,
	ERR_EXTENSION,
	ERR_OPEN,
	ERR_MALLOC,
	ERR_DUPLICATE,
	ERR_MISSING_CONFIG,
	ERR_INVALID_TEXTURE,
	ERR_INVALID_COLOR,
	ERR_NO_MAP,
	ERR_INVALID_CHAR,
	ERR_INVALID_CHARACTHER,
	ERR_NO_PLAYER,
	ERR_MULTI_PLAYER,
	ERR_MAP_NOT_CLOSED
}						t_error;

int						parse_file(const char *path, t_parser *parser);

char					*get_error_msg(int code);

#endif
