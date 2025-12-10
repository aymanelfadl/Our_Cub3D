/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelfadl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:00:11 by aelfadl           #+#    #+#             */
/*   Updated: 2025/12/10 16:31:29 by aelfadl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	update_animations(t_game *game)
{
	static int	frame_counter;
	static int	current_frame;

	if (game->shoot)
	{
		frame_counter++;
		if (frame_counter >= 2)
		{
			frame_counter = 0;
			current_frame++;
			if (current_frame >= 5)
			{
				current_frame = 0;
				game->shoot = 0;
			}
		}
	}
	game->current_frame = current_frame;
}

static void	draw_scaled_pixel(t_game *game, t_texture tex, int x, int y)
{
	int	i;
	int	j;
	int	scale;
	int	color;

	scale = 3;
	i = 0;
	color = get_texture_color(tex.img, y, x);
	if (color != 0x000000)
	{
		while (i < scale)
		{
			j = 0;
			while (j < scale)
			{
				my_mlx_pixel_put(&game->frame, (WINDOW_WIDTH - tex.width
						* scale) / 2 + x * scale + j, (WINDOW_HEIGHT
						- tex.height * scale) + y * scale + i, color);
				j++;
			}
			i++;
		}
	}
}

void	draw_hand(t_game *game, t_texture tex)
{
	int	x;
	int	y;

	y = 0;
	while (y < tex.height)
	{
		x = 0;
		while (x < tex.width)
		{
			draw_scaled_pixel(game, tex, x, y);
			x++;
		}
		y++;
	}
}
