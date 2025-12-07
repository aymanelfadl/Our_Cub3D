#include "cub3D.h"

static void	init_img(t_game *game, t_img *img, int width, int height)
{
	if (img->mlx_img)
		mlx_destroy_image(game->mlx, img->mlx_img);
	img->mlx_img = mlx_new_image(game->mlx, width, height);
	img->addr = mlx_get_data_addr(img->mlx_img, &img->bpp, &img->line_len,
			&img->endian);
}

static void	compute_ray_direction(t_game *game, int column)
{
	game->cfg.player.ray.camera_x = 2.0f * column / (float)WINDOW_WIDTH - 1.0f;
	game->cfg.player.ray.ray_x = game->cfg.player.dir_x
		+ game->cfg.player.ray.camera_x * game->cfg.player.plane_x;
	game->cfg.player.ray.ray_y = game->cfg.player.dir_y
		+ game->cfg.player.ray.camera_x * game->cfg.player.plane_y;
	if (game->cfg.player.ray.ray_x != 0)
		game->cfg.player.ray.next_cell_x = fabsf(1.0f
				/ game->cfg.player.ray.ray_x);
	else
		game->cfg.player.ray.next_cell_x = 1e30f;
	if (game->cfg.player.ray.ray_y != 0)
		game->cfg.player.ray.next_cell_y = fabsf(1.0f
				/ game->cfg.player.ray.ray_y);
	else
		game->cfg.player.ray.next_cell_y = 1e30f;
}

static void	render(t_game *game)
{
	int	x;
	int	line_height;

	draw_background(game, color_to_int(game->cfg.ceiling),
		color_to_int(game->cfg.floor));
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		compute_ray_direction(game, x);
		line_height = start_dda(game);
		if (!game->cfg.player.ray.hit.side)
			game->z_buffer[x] = game->cfg.player.ray.distance_x;
		else
			game->z_buffer[x] = game->cfg.player.ray.distance_y;
		draw_vertical_line(game, x, get_texture(game), line_height);
		x++;
	}
	draw_sprites(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win, game->frame.mlx_img, 0, 0);
	mlx_put_image_to_window(game->mlx, game->win, game->minimap.mlx_img, 10,
		10);
}

int	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_animations(game);
	init_img(game, &game->frame, WINDOW_WIDTH, WINDOW_HEIGHT);
	render(game);
	return (0);
}

int	start_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (fprintf(stderr, "Error\nFailed to initialize MLX\n"), 1);
	if (texture_load_all(game->mlx, &game->cfg) != 0)
		return (fprintf(stderr, "Error\nFailed to load textures\n"), 1);
	if (!load_door_texture(game))
		return (fprintf(stderr, "Error:\nFailed to load the door texture\n"),
			1);
	if (load_sprite_textures(game) != 0)
		return (fprintf(stderr, "Warning: Failed to load sprite frames\n"), 1);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->win)
		return (fprintf(stderr, "mlx_new_window failed\n"), 1);
	init_img(game, &game->frame, WINDOW_WIDTH, WINDOW_HEIGHT);
	init_img(game, &game->minimap, 100, 100);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_hook(game->win, 2, 1L << 0, handle_key, game);
	mlx_hook(game->win, 6, 1L << 6, mouse_move, game);
	mlx_hook(game->win, 17, 0, close_game, game);
	mlx_mouse_hide(game->mlx, game->win);
	mlx_loop(game->mlx);
	return (1);
}
