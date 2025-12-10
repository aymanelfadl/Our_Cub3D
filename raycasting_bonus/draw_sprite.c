#include "cub3D.h"

// static void	calculate_vertical_bounds(t_sprite_render *render)	
// {
// 	render->start_y = (WINDOW_HEIGHT - render->sprite_width) / 2;
// 	render->end_y = (WINDOW_HEIGHT + render->sprite_width) / 2;
// 	if (render->start_y < 0)
// 		render->start_y = 0;
// 	if (render->end_y >= WINDOW_HEIGHT)
// 		render->end_y = WINDOW_HEIGHT - 1;
// }

// static void	calculate_horizontal_bounds(t_sprite_render *render)
// {
// 	render->sprite_left = render->screen_x - render->sprite_width / 2;
// 	render->end_x = render->screen_x + render->sprite_width / 2;
// 	render->start_x = render->sprite_left;
// 	if (render->start_x < 0)
// 		render->start_x = 0;
// 	if (render->end_x >= WINDOW_WIDTH)
// 		render->end_x = WINDOW_WIDTH - 1;
// }

// static void	draw_sprite_stripe(t_game *game, int x, t_sprite_render *render,
// 		t_img texture)
// {
// 	float			tex_step;
// 	float			tex_pos;
// 	unsigned int	color;
// 	int				y;

// 	if (render->depth > game->z_buffer[x])
// 		return ;
// 	tex_step = (float)texture.height / render->sprite_width;
// 	tex_pos = 0;
// 	y = render->start_y;
// 	while (y < render->end_y)
// 	{
// 		color = get_texture_color(texture, (int)tex_pos, render->tex_x);
// 		if (color != 0x000000)
// 			my_mlx_pixel_put(&game->frame, x, y, color);
// 		tex_pos += tex_step;
// 		y++;
// 	}
// }

// static void	draw_sprite_columns(t_game *game, t_sprite_render *render,
// 		t_img texture)
// {
// 	int	offset_x;

// 	render->tex_step_x = (float)texture.width / render->sprite_width;
// 	while (render->start_x < render->end_x)
// 	{
// 		offset_x = render->start_x - render->sprite_left;
// 		render->tex_x = (int)(offset_x * render->tex_step_x);
// 		if (render->tex_x >= texture.width)
// 			render->tex_x = texture.width - 1;
// 		draw_sprite_stripe(game, render->start_x, render, texture);
// 		render->start_x++;
// 	}
// }

// void	render_sprite(t_game *game, int sprite_index)
// {
// 	t_sprite	*sprite;
// 	t_img		texture;

// 	sprite = &game->sprites[sprite_index];
// 	transform_to_camera_space(game, sprite);
// 	if (sprite->render.depth <= 0)
// 		return ;
// 	calculate_vertical_bounds(&sprite->render);
// 	calculate_horizontal_bounds(&sprite->render);
// 	texture = sprite->sprite_textures[sprite->current_frame].img;
// 	draw_sprite_columns(game, &sprite->render, texture);
// }
