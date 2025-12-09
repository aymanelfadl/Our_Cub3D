#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "../libft/libft.h"
# include <mlx.h>

# define TEXTURE_COUNT 4
# define FOV_PLANE 60
# ifndef M_PI
# define M_PI 3.14159265358979323846
# endif

# define WINDOW_WIDTH 1800
# define WINDOW_HEIGHT 1000

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_E 101
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define COLLISION_CONST 0.1
# define MOVE_SPEED 0.08f
# define ROT_SPEED 0.05f
# define TILE_SIZE 10
# define RADIUS 5

typedef enum e_direction
{
	NO,
	SO,
	WE,
	EA
}   t_direction;

typedef struct s_color
{
	int red;
	int green;
	int blue;
}   t_color;

typedef struct s_img
{
	void    *mlx_img;
	char    *addr;
	int     bpp;
	int     line_len;
	int     endian;
	int     width;
	int     height;
}   t_img;

typedef struct s_texture
{
	t_direction id;
	char        *path;
	t_img       img;
	int         width;
	int         height;
	int         loaded;
}   t_texture;

typedef struct s_hit
{
	float   hit_x;
	float   hit_y;
	int     is_hit;
	int     side;
}   t_hit;

typedef struct s_ray
{
	float           camera_x;
	float           ray_x;
	float           ray_y;
	float           next_cell_x;
	float           next_cell_y;
	int             step_x;
	int             step_y;
	float           distance_x;
	float           distance_y;
	struct s_hit    hit;
}   t_ray;

typedef struct s_player
{
	float       pos_x;
	float       pos_y;
	float       dir_x;
	float       dir_y;
	float       plane_x;
	float       plane_y;
	t_direction direction;
	t_ray       ray;
}   t_player;

typedef struct s_map
{
	int     width;
	int     height;
	char    **grid;
}   t_map;

typedef struct s_sprite_render
{
	int     screen_x;
	int     sprite_width;
	int     sprite_left;
	int     start_x;
	int     end_x;
	float	tex_step_x;
	int		tex_x;
	int     start_y;
	int     end_y;
	float   depth;
}   t_sprite_render;

typedef struct s_sprite
{
	float               x;
	float               y;
	float               distance;
	int                 current_frame;
	t_texture       	sprite_textures[4];
	t_sprite_render     render;
}   t_sprite;

typedef struct s_door
{
	int map_x;
	int map_y;
	int is_open;
}   t_door;

typedef struct s_config
{
	t_texture   textures[TEXTURE_COUNT];
    char      *no_texture;
    char      *so_texture;
    char      *we_texture;
    char      *ea_texture;
	t_texture   door_texture;
	t_color     floor;
	t_color     ceiling;
	int         f_set;
	int         c_set;
	t_map       map;
	t_player    player;
}   t_config;

typedef struct s_game
{
	void        *mlx;
	void        *win;
	t_img       frame;
	t_img       minimap;
	t_config    cfg;
	t_sprite    *sprites;
	int         sprite_count;
	float       z_buffer[WINDOW_WIDTH];
	t_door      *doors;
	t_texture	hand_texture[4];
	int         door_count;
}   t_game;

int start_game(t_game *game);
int game_loop(void *param);
int start_dda(t_game *game);
void	transform_to_camera_space(t_game *game, t_sprite *sprite);
void	update_animations(t_game *game);
void	update_sprites(t_game *game);

int  texture_load_all(void *mlx, t_config *cfg);
void texture_free_all(void *mlx, t_config *cfg);
int load_texture(void *mlx, t_texture *tex);
int load_sprite_textures(t_game *game);
int load_door_texture(t_game *game);
t_img get_texture(t_game *game);
t_img get_proper_texture(t_texture *texs, t_direction dir);
float get_texture_y(int *drawing_start, float text_step);
int get_wall_hit(t_game *game, t_img texture);
unsigned int get_texture_color(t_img texture, int tex_y, int tex_x);

void draw_background(t_game *game, int ceil_color, int floor_color);
void draw_minimap(t_game *game);
void draw_sprites(t_game *game);
void render_sprite(t_game *game, int sprite_index);
void draw_vertical_line(t_game *game, int x, t_img texture, int line_height);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
int color_to_int(t_color c);

int handle_key(int key, t_game *game);
void player_movement(int key, t_game *game);
int mouse_move(int x, int y, void *game);
void apply_movement(t_game *game, float new_x, float new_y);
int close_game(t_game *game);

void parser_release_config(t_config *cfg);

t_door *find_door_at(t_game *game, int x, int y);
void toggle_door(t_game *game);
int	check_door(t_game *game, float x, float y);
int is_player_cell(char c);

int load_hand_texture(t_game *game);

#endif