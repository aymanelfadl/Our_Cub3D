#include "cub3D.h"

t_door *find_door_at(t_game *game, int x, int y)
{
    int i;

    i = 0;
    while (i < game->door_count)
    {
        if (game->doors[i].map_x == x && game->doors[i].map_y == y)
            return (&game->doors[i]);
        i++;
    }
    return (NULL);
}

void toggle_door(t_game *game)
{
    int check_x;
    int check_y;
    t_door *door;

    check_x = (int)(game->cfg.player.pos_x + game->cfg.player.dir_x * 1.0f);
    check_y = (int)(game->cfg.player.pos_y + game->cfg.player.dir_y * 1.0f);
    door = find_door_at(game, check_x, check_y);
    if (door != NULL)
    {
        door->is_open = !door->is_open;        
        render(game);
    }
}
