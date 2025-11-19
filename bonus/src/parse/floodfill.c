#include <stdlib.h>
#include "parser_internal.h"

#define IS_WALKABLE(c) ((c) == '0' || (c) == '2')

typedef struct s_point
{
    int x;
    int y;
}   t_point;

static int check_bounds(t_map *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height)
        return (PARSE_ERR_MAP_OPEN);
    if (map->grid[y][x] == ' ')
        return (PARSE_ERR_MAP_OPEN);
    return (PARSE_OK);
}

static void push_cell(t_map *map, char *visited, t_point *stack, int *top,
        int x, int y)
{
    int idx;

    idx = y * map->width + x;
    if (!visited[idx])
    {
        visited[idx] = 1;
        *top += 1;
        stack[*top].x = x;
        stack[*top].y = y;
    }
}

static int explore_neighbours(t_map *map, t_point cur, char *visited,
        t_point *stack, int *top)
{
    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};
    int             i;
    int             nx;
    int             ny;
    int             code;

    i = 0;
    while (i < 4)
    {
        nx = cur.x + dx[i];
        ny = cur.y + dy[i];
        code = check_bounds(map, nx, ny);
        if (code != PARSE_OK)
            return (code);
        if (IS_WALKABLE(map->grid[ny][nx]))
            push_cell(map, visited, stack, top, nx, ny);
        i++;
    }
    return (PARSE_OK);
}

static int flood_fill(t_map *map, int sx, int sy, char *visited)
{
    t_point *stack;
    int     capacity;
    int     top;
    t_point cur;
    int     code;

    capacity = map->width * map->height;
    stack = malloc(sizeof(t_point) * capacity);
    if (!stack)
        return (PARSE_ERR_ALLOC);
    top = 0;
    stack[0].x = sx;
    stack[0].y = sy;
    visited[sy * map->width + sx] = 1;
    code = PARSE_OK;
    while (top >= 0 && code == PARSE_OK)
    {
        cur = stack[top--];
        code = explore_neighbours(map, cur, visited, stack, &top);
    }
    free(stack);
    return (code);
}

static int visit_cell(t_map *map, char *visited, int x, int y)
{
    if (!IS_WALKABLE(map->grid[y][x]))
        return (PARSE_OK);
    if (visited[y * map->width + x])
        return (PARSE_OK);
    return (flood_fill(map, x, y, visited));
}

static int scan_map(t_map *map, char *visited)
{
    int y;
    int x;
    int code;

    y = 0;
    while (y < map->height)
    {
        x = 0;
        while (x < map->width)
        {
            code = visit_cell(map, visited, x, y);
            if (code != PARSE_OK)
                return (code);
            x++;
        }
        y++;
    }
    return (PARSE_OK);
}

int parser_validate_enclosure(t_map *map)
{
    char    *visited;
    int     count;
    int     code;

    count = map->height * map->width;
    visited = ft_calloc(count, sizeof(char));
    if (!visited)
        return (PARSE_ERR_ALLOC);
    code = scan_map(map, visited);
    free(visited);
    return (code);
}
