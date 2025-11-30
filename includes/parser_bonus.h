/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:50:25 by Yoares            #+#    #+#             */
/*   Updated: 2025/11/30 21:21:32 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "parser.h"
# include "cub3D.h"
# include "parser_internal.h"
/* Bonus error codes (extend the enum from parser.h) */
typedef enum e_bonus_error
{
    ERR_INVALID_DOOR = 20,
    ERR_INVALID_SPRITE,
    ERR_DOOR_TEXTURE,
    ERR_SPRITE_TEXTURE,
    ERR_BONUS_CONFIG
} t_bonus_error;

/* Extended parser structure for bonus */
typedef struct s_parser_bonus
{
    t_parser base;  // mandatory parse
    char *door_texture;
    char **sprite_textures;
    int sprite_texture_count;
} t_parser_bonus;

/* Bonus validation helpers */
int     is_bonus_char(char c);
int     is_door_char(char c);
int     is_sprite_char(char c);
int     validate_door_placement(t_map *map, int x, int y);
int     validate_sprite_placement(t_map *map, int x, int y);
/* Bonus parsing functions */
int parse_file_bonus(const char *path, t_parser_bonus *parser);
int detect_doors(t_parser_bonus *parser);
int detect_sprites(t_parser_bonus *parser);
int build_bonus_entities(t_parser_bonus *parser);
int validate_bonus_chars(t_parser_bonus *parser);
char    *get_bonus_error_msg(int code);

#endif
