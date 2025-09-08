#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include "../libft/libft.h"

#include <stdio.h>
#include <mlx.h>
#include <math.h>

#define TEXTURE_COUNT 4
#define FOV_PLANE 60
#define M_PI 3.14159265358979323846


#define WINDOW_WIDTH 50
#define WINDOW_HEIGHT 50

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
    t_color   floor_color;
    t_color   ceiling_color;
    t_map     map;
    t_player  player;
    int       have_floor;
    int       have_ceiling;
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

// Main Parsing
t_game *init_game(const char *file);
void game_info(int fd, t_game *game);

// Textures
int is_texture(char *id);
void which_identifier(t_texture *tex, char *key);
t_texture get_texture(char **map);
int game_config(t_game *game, char **map);

// Colors
char *merge_map_colors(char **map);
int parse_color_value(char *str, t_color *color);
void get_color_pointers(t_game *game, int is_floor, int **flag, t_color **color);
void parse_apply_color(t_color *color, char **map);
int handle_color(t_game *game, char **map, int is_floor);
int in_range(int n, int min, int max);

// Map
int *get_map_dimension(const char *file);
int valid_map(t_game *game);
int game_map(t_game *game, char *line);
void normalize_map(t_game *game);
int scan_map_cells(t_game *game);
int is_close_map(t_game *game);
t_direction get_direction(t_game *game, char c);

// Utils
int ft_split_size(char **arr);
int ft_strcmp(const char *s1, const char *s2);
int wrap_ft_aoti(const char *str);
char **allocate_map_grid(int height, int width);
void print_err(const char *msg);
void ft_free_split(char **arr);
int check_extension(const char *file, const char *suffix);
void check_duplicate(int *flag, int is_floor);
int check_extension(const char *file, const char *suffix);
int is_config(char *str);
int is_valid_map_char(char c);
int is_it_player(char c);


// 
void start_game(t_game *game);


void debug_print_split(char **split, const char *original_line);


#endif 