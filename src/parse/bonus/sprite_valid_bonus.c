#include "parser_internal.h"
#include "parser_bonus.h"

int	is_sprite_char(char c)
{
    return (c == '2');
}
/* Check if character is a space or end of string */
static	int	is_space_or_invalid(char c)
{
    return (c == ' ' || c == '\0');
}

/* Check if character is walkable for sprite placement */
static int	is_walkable_for_sprite(char c)
{
    return (c == '0' || c == 'N' || c == 'S' || 
            c == 'E' || c == 'W' || c == 'D');
}

static int	count_accessible_sides(char up, char down, char left, char right)
{
    int	count;

    count = 0;
    if (is_walkable_for_sprite(up))
        count++;
    if (is_walkable_for_sprite(down))
        count++;
    if (is_walkable_for_sprite(left))
        count++;
    if (is_walkable_for_sprite(right))
        count++;
    return (count);
}

int	validate_sprite_placement(t_map *map, int j, int i)
{
    char	up;
    char	down;
    char	left;
    char	right;

    if (i <= 0 || j <= 0 || j >= map->width - 1 || i >= map->height - 1)
        return (ERR_INVALID_SPRITE);
    up = map->grid[i - 1][j];
    down = map->grid[i + 1][j];
    left = map->grid[i][j - 1];
    right = map->grid[i][j + 1];
    if (is_space_or_invalid(up) || is_space_or_invalid(down) ||
        is_space_or_invalid(left) || is_space_or_invalid(right))
        return (ERR_INVALID_SPRITE);
    if (count_accessible_sides(up, down, left, right) < 2)
        return (ERR_INVALID_SPRITE);
    return (OK);
}
