/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:31:10 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:31:10 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "cub3D.h"
# include "parser.h"
# include "parser_internal.h"

typedef enum e_bonus_error
{
	ERR_INVALID_DOOR = 20,
	ERR_INVALID_SPRITE,
	ERR_DOOR_TEXTURE,
	ERR_SPRITE_TEXTURE,
	ERR_BONUS_CONFIG
}				t_bonus_error;

typedef struct s_parser_bonus
{
	t_parser	base;
	char		*door_texture;
	char		**sprite_textures;
	int			sprite_texture_count;
}				t_parser_bonus;

int				is_bonus_char(char c);
int				is_door_char(char c);
int				validate_door_placement(t_map *map, int x, int y);

int				parse_file_bonus(const char *path, t_parser_bonus *parser);
int				detect_doors(t_parser_bonus *parser);
int				validate_bonus_chars(t_parser_bonus *parser);
char			*get_bonus_error_msg(int code);

#endif
