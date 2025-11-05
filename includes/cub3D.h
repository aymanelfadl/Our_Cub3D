#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include "../libft/libft.h"

#include <stdio.h>
#include <mlx.h>
#include <math.h>

#define TEXTURE_COUNT 4
#define FOV_PLANE 66
#define M_PI 3.14159265358979323846

#define MOVE_SPEED 0.2f
#define ROT_SPEED 0.05f 

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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
    int     width;
    int     height;
} t_img;

typedef struct s_texture {
    t_direction id;   
    char        *path;
    t_img       img;
} t_texture;


typedef struct s_hit
{
    float hit_x;
    float hit_y;
    int is_hit;
    int side;
} t_hit;


typedef struct s_ray
{
    float camera_x;      
    float ray_x;             
    float ray_y;             
    float next_cell_x;   
    float next_cell_y;
    float distance_x;
    float distance_y;
    int step_x;
    int step_y;
    t_hit hit;     
} t_ray;

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
    t_ray ray;
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

typedef struct s_game {
    void     *mlx;
    void     *win;
    t_img    frame;
    t_config cfg;
} t_game;

// Main Parsing
t_game *init_game(const char *file);
int game_info(int fd, t_game *game);

// Textures
int game_config(t_game *game, char **map);
t_direction get_direction(t_game *game, char c);

// Colors
int handle_color(t_game *game, char **map, int is_floor);
int in_range(int n, int min, int max);

// Map
int *get_map_dimension(const char *file);
int game_map(t_game *game, char *line);
int normalize_map(t_game *game);
int valid_map(t_game *game);
int scan_map_cells(t_game *game);
int is_close_map(t_game *game);

// Utils
int ft_split_size(char **arr);
int ft_strcmp(const char *s1, const char *s2);
int wrap_ft_aoti(const char *str);
char **allocate_map_grid(int height, int width);
void ft_free_split(char **arr);
int check_extension(const char *file, const char *suffix);
int is_config(char *str);
int is_valid_map_char(char c);
int is_it_player(char c);
void ft_free_texture(t_game *game);
void ft_free_textures_img(t_game *game);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
int	color_to_int(t_color c);
t_img get_proper_texture(t_texture *texs, t_direction dir);

// RayCasting  
void render(t_game *game);
int start_game(t_game *game);
void end_game(t_game *game);

// DDA
void perform_dda(t_game *game, int *map_y, int *map_x);
void init_dda(t_game *game, int map_y, int map_x);
void compute_ray_direction(t_game *game, int column);

// Drawing
void draw_background(t_game *game, int ceil_color, int floor_color);
void draw_vertical_line(t_game *game, int x);


// Events
int handle_key(int key, t_game *game);
void player_movement(int key, t_game *game);
int close_game(t_game *game);


// 


void debug_print_split(char **split, const char *original_line);


#endif 