/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhoussi <ykhoussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:50:25 by Yoares            #+#    #+#             */
/*   Updated: 2025/11/18 19:07:27 by ykhoussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

# include "parser.h"

// Define bonus characters
#define CHAR_DOOR 'D'
#define CHAR_SPRITE '2'

// Door position structure
typedef struct s_door_pos
{
    int x;
    int y;
}   t_door_pos;

// Sprite position structure  
typedef struct s_sprite_pos
{
    float x;  // Use float for center positioning (x + 0.5)
    float y;  // Use float for center positioning (y + 0.5)
}   t_sprite_pos;

// Extended config (inherits from t_config)
typedef struct s_config_bonus
{
    t_config        base;               // All mandatory fields
    t_door_pos      *door_positions;    // Array of door positions
    int             door_count;         // Number of doors
    t_sprite_pos    *sprite_positions;  // Array of sprite positions
    int             sprite_count;       // Number of sprites
}   t_config_bonus;

// Function prototypes
int parser_validate_map_chars_bonus(const char *line);
int parser_collect_doors(t_config *cfg);
int parser_collect_sprites(t_config *cfg);
int parse_cub_file_bonus(const char *path, t_game *game);

#endif
