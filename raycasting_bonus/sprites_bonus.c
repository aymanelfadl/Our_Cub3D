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

void update_animations(t_game *game)
{
    static int frame_counter;
    static int current_frame;
    
    if (game->shoot)
    {
        frame_counter++;
        if (frame_counter >= 10)
        {
            frame_counter = 0;
            current_frame++;
            if (current_frame >= 4)
            {
                current_frame = 0;
                game->shoot = 0;
            }
        }
    }
	game->current_frame = current_frame;
}

void draw_hand(t_game *game, t_texture tex)
{
    int x;
	int y;
	int color;

	y = 0;
    while ( y < tex.height)
    {
		x = 0;
        while (x < tex.width)
        {
			color = get_texture_color(tex.img, y, x);
            if (color != 0x000000)
                my_mlx_pixel_put(&game->frame, (WINDOW_WIDTH - tex.width ) / 2 + x, 
                                 WINDOW_HEIGHT - tex.height + y, color);
			x++;
		}
		y++;
    }
}