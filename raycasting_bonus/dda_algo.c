#include "cub3D.h"

void	compute_ray_direction(t_game *game, int column)
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

// next cell is the grid space.
void	init_dda(t_game *game, int map_y, int map_x)
{
	if (game->cfg.player.ray.ray_x < 0)
	{
		game->cfg.player.ray.step_x = -1;
		game->cfg.player.ray.distance_x = (game->cfg.player.pos_x - map_x)
			* game->cfg.player.ray.next_cell_x;
	}
	else
	{
		game->cfg.player.ray.step_x = 1;
		game->cfg.player.ray.distance_x = (map_x + 1.0f
				- game->cfg.player.pos_x) * game->cfg.player.ray.next_cell_x;
	}
	if (game->cfg.player.ray.ray_y < 0)
	{
		game->cfg.player.ray.step_y = -1;
		game->cfg.player.ray.distance_y = (game->cfg.player.pos_y - map_y)
			* game->cfg.player.ray.next_cell_y;
	}
	else
	{
		game->cfg.player.ray.step_y = 1;
		game->cfg.player.ray.distance_y = (map_y + 1.0f
				- game->cfg.player.pos_y) * game->cfg.player.ray.next_cell_y;
	}
}

void	perform_dda(t_game *game, int *map_y, int *map_x)
{
	t_door	*door;

	game->cfg.player.ray.hit.is_hit = 0;
	while (!game->cfg.player.ray.hit.is_hit)
	{
		if (game->cfg.player.ray.distance_x < game->cfg.player.ray.distance_y)
		{
			game->cfg.player.ray.distance_x += game->cfg.player.ray.next_cell_x;
			(*map_x) += game->cfg.player.ray.step_x;
			game->cfg.player.ray.hit.side = 0;
		}
		else
		{
			game->cfg.player.ray.distance_y += game->cfg.player.ray.next_cell_y;
			(*map_y) += game->cfg.player.ray.step_y;
			game->cfg.player.ray.hit.side = 1;
		}
		if (game->cfg.map.grid[*map_y][*map_x] == '1')
		{
			game->cfg.player.ray.distance_x -= game->cfg.player.ray.next_cell_x;
			game->cfg.player.ray.distance_y -= game->cfg.player.ray.next_cell_y;
			game->cfg.player.ray.hit.is_hit = 1;
		}
		door = find_door_at(game, *map_x, *map_y);
		if (door != NULL && !door->is_open)
		{
			game->cfg.player.ray.distance_x -= game->cfg.player.ray.next_cell_x;
			game->cfg.player.ray.distance_y -= game->cfg.player.ray.next_cell_y;
			game->cfg.player.ray.hit.is_hit = 2;
		}
	}
}
