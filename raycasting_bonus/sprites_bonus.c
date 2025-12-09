/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:00:11 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/07 22:00:11 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	calculate_sprite_distances(t_game *game)
{
	int		i;
	float	dx;
	float	dy;

	i = 0;
	while (i < game->sprite_count)
	{
		dx = game->sprites[i].x - game->cfg.player.pos_x;
		dy = game->sprites[i].y - game->cfg.player.pos_y;
		game->sprites[i].distance = pow(dx, 2) + pow(dy, 2);
		i++;
	}
}

static void	sort_sprites(t_sprite *sprites, int count)
{
	int			i;
	int			j;
	t_sprite	tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (sprites[j].distance < sprites[j + 1].distance)
			{
				tmp = sprites[j];
				sprites[j] = sprites[j + 1];
				sprites[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	transform_to_camera_space(t_game *game, t_sprite *sprite)
{
	float	sprite_x;
	float	sprite_y;
	float	det;
	float	inv_det;
	float	side;

	sprite_x = sprite->x - game->cfg.player.pos_x;
	sprite_y = sprite->y - game->cfg.player.pos_y;
	
	det = (game->cfg.player.plane_x * game->cfg.player.dir_y)
		- (game->cfg.player.dir_x * game->cfg.player.plane_y);
	inv_det = 1.0 / det;
	side = inv_det * (game->cfg.player.dir_y * sprite_x - game->cfg.player.dir_x
			* sprite_y);
	sprite->render.depth = inv_det * (-game->cfg.player.plane_y * sprite_x
			+ game->cfg.player.plane_x * sprite_y);
	sprite->render.screen_x = (int)((WINDOW_WIDTH / 2) * (1 + side
				/ sprite->render.depth));
	sprite->render.sprite_width = (int)(WINDOW_HEIGHT / sprite->render.depth);
}

void	update_animations(t_game *game)
{
	static int	frame_counter;
	int			i;

	frame_counter = frame_counter + 1;
	if (frame_counter >= 30)
	{
		frame_counter = 0;
		i = 0;
		while (i < game->sprite_count)
		{
			game->sprites[i].current_frame++;
			if (game->sprites[i].current_frame == 4)
				game->sprites[i].current_frame = 0;
			i++;
		}
	}
}

void	draw_sprites(t_game *game)
{
	int	i;

	calculate_sprite_distances(game);
	sort_sprites(game->sprites, game->sprite_count);
	i = 0;
	while (i < game->sprite_count)
	{
		render_sprite(game, i);
		i++;
	}
}
