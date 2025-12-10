#include "cub3D.h"

// int is_player_cell(char c)
// {
//     return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
// }

// void	move_sprite_towards_player(t_game *game, t_sprite *sprite)
// {
//     double	dx;
//     double	dy;
//     double	distance;
//     double	new_x;
//     double	new_y;
    
//     dx = game->cfg.player.pos_x - sprite->x;
//     dy = game->cfg.player.pos_y - sprite->y;
//     distance = sqrt(dx * dx + dy * dy);
    
//     if (distance < 1)
// 	{
// 		return ;
// 	}
// 	new_x = sprite->x + (dx / distance) * 0.005;
//     new_y = sprite->y + (dy / distance) * 0.005;
 	
// 	if (game->cfg.map.grid[(int)new_y][(int)new_x] == '0'
// 		|| game->cfg.map.grid[(int)new_y][(int)new_x] == '2' 
//         || is_player_cell(game->cfg.map.grid[(int)new_y][(int)new_x]))
//     {
//         sprite->x = new_x;
//         sprite->y = new_y;
//     }
// }
// void	update_sprites(t_game *game)
// {
//     int	i;

//     i = 0;
//     while (i < game->sprite_count)
//     {
//         move_sprite_towards_player(game, &game->sprites[i]);
//         i++;
//     }
// }

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
                my_mlx_pixel_put(&game->frame, (WINDOW_WIDTH - game->hand_texture[game->current_frame].width)/2 + x, 
                                 WINDOW_HEIGHT - game->hand_texture[game->current_frame].height + y, color);
			x++;
		}
		y++;
    }
}