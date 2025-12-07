#include "cub3D.h"

static int	is_blocked(t_game *game, float x, float y)
{
	if (game->cfg.map.grid[(int)(y - COLLISION_CONST)][(int)(x
			- COLLISION_CONST)] == '1')
		return (1);
	if (game->cfg.map.grid[(int)(y - COLLISION_CONST)][(int)(x
			+ COLLISION_CONST)] == '1')
		return (1);
	if (game->cfg.map.grid[(int)(y + COLLISION_CONST)][(int)(x
			- COLLISION_CONST)] == '1')
		return (1);
	if (game->cfg.map.grid[(int)(y + COLLISION_CONST)][(int)(x
			+ COLLISION_CONST)] == '1')
		return (1);
	if (check_door(game, x, y))
		return (1);
	return (0);
}

void	apply_movement(t_game *game, float new_x, float new_y)
{
	float old_x;
	float old_y;

	old_x = game->cfg.player.pos_x;
	old_y = game->cfg.player.pos_y;
	if (!is_blocked(game, new_x, new_y))
	{
		game->cfg.player.pos_x = new_x;
		game->cfg.player.pos_y = new_y;
		return ;
	}
	if (!is_blocked(game, new_x, old_y))
	{
		game->cfg.player.pos_x = new_x;
		return ;
	}
	if (!is_blocked(game, old_x, new_y))
	{
		game->cfg.player.pos_y = new_y;
		return ;
	}
}
