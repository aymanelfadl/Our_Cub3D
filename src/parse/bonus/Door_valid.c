#include "parser_internal.h"
#include "parser_bonus.h"

int	is_door_char(char c)
{
	return (c == 'D');
}

static int is_player_char(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int validate_door_placement(t_map *map, int j, int i)
{
    char	up;
    char	down;
    char	left;
    char	right;
    int     wall_count;
    int     open_count;

	if (j <= 0 || i <= 0 || j >= map->width - 1 || i >= map->height - 1)
        return (ERR_INVALID_DOOR);
	
    wall_count = 0;
    open_count = 0;
    up = map->grid[i - 1][j];
    down = map->grid[i + 1][j];
    left = map->grid[i][j - 1];
    right = map->grid[i][j + 1];
    if (up == '1')
        wall_count++;
    if (down == '1')
        wall_count++;
    if (left == '1')
        wall_count++;
    if (right == '1')
        wall_count++;
    
    if (up == '0' || is_player_char(up)) open_count++;
    if (down == '0' || is_player_char(down)) open_count++;
    if (left == '0' || is_player_char(left)) open_count++;
    if (right == '0' || is_player_char(right)) open_count++;

    if (wall_count >= 2 && open_count >= 2)
        return (OK);

    return (ERR_INVALID_DOOR);
}
