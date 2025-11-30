#ifndef CUB3D_H
#define CUB3D_H

#include <unistd.h>
#include "../libft/libft.h"

#include <stdio.h>
#include <stdlib.h>
#include "../minilibx-linux/mlx.h"
#include <math.h>

#define TEXTURE_COUNT 4
#define FOV_PLANE 60
#define M_PI 3.14159265358979323846


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define EVENT_KEY_PRESS 2
#define EVENT_DESTROY 17
#define MASK_KEY_PRESS (1L << 0)

#define KEY_ESC 65307
#define KEY_W 119
#define KEY_S 115
#define KEY_A 97
#define KEY_D 100
#define KEY_Q 113
#define KEY_E 101
#define KEY_LEFT 65361
#define KEY_RIGHT 65363

#define MOVE_SPEED 0.08f
#define ROT_SPEED 0.05f

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
    char *path;        /* path set by parser */
    t_img   img;       /* MLX image + addr/bpp/line_len/endian */
    int     width;
    int     height;
    int     loaded;    /* 0/1 flag */
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
    int   step_x;
    int   step_y;
    float distance_x;
    float distance_y;
    struct s_hit hit;
} t_ray;

typedef struct s_player {
    float pos_x;
    float pos_y;
    float dir_x;
    float dir_y;
    float plane_x;
    float plane_y;
    t_direction direction;
    t_ray    ray;
} t_player;

typedef struct s_map {
    int   width;
    int   height;
    int   sprite_count;
    // int   door_count;
    char **grid;
} t_map;

/* bonus  bonus*/
typedef struct s_sprite
{
    float x;
    float y;
    float distance;
    int current_frame;     // Current animation frame (0, 1, 2, ...)
    int frame_count;       // Total number of frames
    float frame_timer;     // Time accumulator for frame switching
    float frame_duration;  // Time per frame (lower = faster animation)
} t_sprite;

typedef struct s_sprite_render
{
    int sprite_height;
    int sprite_width;
    int sprite_screen_x;
    int draw_start_x;
    int draw_end_x;
    int draw_start_y;
    int draw_end_y;
} t_sprite_render;

typedef struct s_door
{
    int map_x;
    int map_y;
    int is_open;    // 0 = closed, 1 = open
} t_door;

typedef struct s_game {
    void     *mlx;
    void     *win;
    t_img    frame;
    t_img    minimap;
    t_config cfg;
    t_sprite *sprites;
    int       sprite_count;
    float     z_buffer[WINDOW_WIDTH];
    t_img     sprite_textures[4];  // Array for animation frames (4 frames max)
    int       sprite_frame_count;   // How many frames loaded
    t_door   *doors;
    int       door_count;
    t_img     door_texture;
} t_game;
/* bonus  bonus*/

typedef struct s_config
{
    t_texture textures[TEXTURE_COUNT];
    char      *sprite_path;
	t_color	floor;
	t_color	ceiling;
	int		f_set;
	int		c_set;
    t_map     map;
    t_player  player;
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
} t_config;

// bonus
typedef struct s_sprite
{
    float x;
    float y;
    float distance;
    int current_frame;     // Current animation frame (0, 1, 2, ...)
    int frame_count;       // Total number of frames
    float frame_timer;     // Time accumulator for frame switching
    float frame_duration;  // Time per frame (lower = faster animation)
} t_sprite;

typedef struct s_door
{
    int map_x;
    int map_y;
    int is_open;    // 0 = closed, 1 = open
} t_door;

typedef struct s_game {
    void     *mlx;
    void     *win;
    t_img    frame;
    t_img    minimap;
    t_config cfg;
    t_sprite *sprites;
    int       sprite_count;
    float     z_buffer[WINDOW_WIDTH];
    t_img     sprite_textures[4];  // Array for animation frames (4 frames max)
    int       sprite_frame_count;   // How many frames loaded
    t_door   *doors;
    int       door_count;
    t_img     door_texture;
} t_game;

int start_game(t_game *game);

/* textures API */
int  texture_load_all(void *mlx, t_config *cfg);
void texture_free_all(void *mlx, t_config *cfg);

/* raycasting module functions (exported) */
void draw_background(t_game *game, int ceil_color, int floor_color);
int color_to_int(t_color c);
void compute_ray_direction(t_game *game, int column);
void init_dda(t_game *game, int map_y, int map_x);
void perform_dda(t_game *game, int *map_y, int *map_x);
void draw_vertical_line(t_game *game, int x);
int handle_key(int key, t_game *game);
int close_game(t_game *game);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
t_img get_proper_texture(t_texture *texs, t_direction dir);
void parser_release_config(t_config *cfg);
void player_movement(int key, t_game *game);
void render(t_game *game);


#endif 