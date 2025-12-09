#include "cub3D.h"

int is_player_cell(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	move_sprite_towards_player(t_game *game, t_sprite *sprite)
{
    double	dx;
    double	dy;
    double	distance;
    double	new_x;
    double	new_y;
    
    dx = game->cfg.player.pos_x - sprite->x;
    dy = game->cfg.player.pos_y - sprite->y;
    distance = sqrt(dx * dx + dy * dy);
    
    if (distance < 1)
	{
		return ;
	}
	new_x = sprite->x + (dx / distance) * 0.005;
    new_y = sprite->y + (dy / distance) * 0.005;
 	
	if (game->cfg.map.grid[(int)new_y][(int)new_x] == '0'
		|| game->cfg.map.grid[(int)new_y][(int)new_x] == '2' 
        || is_player_cell(game->cfg.map.grid[(int)new_y][(int)new_x]))
    {
        sprite->x = new_x;
        sprite->y = new_y;
    }
}
void	update_sprites(t_game *game)
{
    int	i;

    i = 0;
    while (i < game->sprite_count)
    {
        move_sprite_towards_player(game, &game->sprites[i]);
        i++;
    }
}