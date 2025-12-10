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

void	update_animations(t_game *game)
{
	static int	frame_counter;

	frame_counter = frame_counter + 1;
	if (frame_counter >= 30)
	{
		frame_counter = 0;
		game->current_frame++;
		if (game->current_frame >= 4)
			game->current_frame = 0;
	}
}

void draw_hand(t_game *game)
{
    int x;
	int y;
	int color;

	y = 0;
    while ( y < game->hand_texture[game->current_frame].height)
    {
		x = 0;
        while (x < game->hand_texture[game->current_frame].width)
        {
			color = get_texture_color(game->hand_texture[game->current_frame].img, y, x);
            if (color != 0x000000)
                my_mlx_pixel_put(&game->frame, (WINDOW_WIDTH - game->hand_texture[game->current_frame].width ) / 2 + x, 
                                 WINDOW_HEIGHT - game->hand_texture[game->current_frame].height + y, color);
			x++;
		}
		y++;
    }
}