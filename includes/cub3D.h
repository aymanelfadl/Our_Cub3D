#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include "../libft/libft.h"

#include <stdio.h>
#include <math.h>

#define TEXTURE_COUNT 4
#define FOV_PLANE 60
#define M_PI 3.14159265358979323846


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

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
    int   sprite_count;
    char **grid;
} t_map;

typedef struct s_player {
    float pos_x;
    float pos_y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    t_direction direction;
} t_player;

typedef struct s_config
{
    t_texture textures[TEXTURE_COUNT];
    char      *sprite_path;
    t_color   floor_color;
    t_color   ceiling_color;
    t_map     map;
    t_player  player;
    int       have_floor;
    int       have_ceiling;
    int       have_sprite;
    int       have_player;
    int       have_resolution;
    int       resolution_width;
    int       resolution_height;
} t_config;


typedef struct s_hit
{
    float hit_x;
    float hit_y;
    int is_hit;
    int side;
} t_hit;


typedef struct s_game {
    void     *mlx;
    void     *win;
    t_img    frame;
    t_config cfg;
} t_game;

void start_game(t_game *game);


#endif 