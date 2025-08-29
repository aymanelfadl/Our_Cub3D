#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include "../libft/libft.h"

#include <stdio.h>

#define TEXTURE_COUNT 4
#define FOV_PLANE 0.66

typedef enum e_direction
{
    NO = 0,
    SO,
    WE,
    EA
} t_direction;

typedef struct s_color
{
    int red;
    int green; 
    int blue;  
} t_color;

typedef struct s_img
{
    void    *mlx_img;
    char    *addr;
    int     width;       
    int     height;
    int     bpp;         
    int     line_len;
    int     endian;
} t_img;

typedef struct s_texture {
    t_direction id;      
    char        *path;
} t_texture;

typedef struct s_map {
    int   width;
    int   height;
    char **grid;
} t_map;

// typedef struct s_player {
//     double x;
//     double y;
//     double dir_x;
//     double dir_y;
//     double plane_x;
//     double plane_y;
// } t_player;

typedef struct s_config {
    t_texture textures[TEXTURE_COUNT];
    t_color   floor_color;
    t_color   ceiling_color;
    t_map     map;
    // t_player  player;
    int       have_floor;
    int       have_ceiling;
    int       player_set;
} t_config;

typedef struct s_game {
    void     *mlx;
    void     *win;
    t_img    frame;
    t_config cfg;
    int      win_w;
    int      win_h;
} t_game;


#endif 