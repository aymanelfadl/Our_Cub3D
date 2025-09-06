#include "cub3D.h"

int ft_split_size(char **arr)
{
    int i = 0;
    if (!arr)
        return 0;
    while (arr[i])
        i++;
    return i;
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int in_range(int n, int min, int max)
{
    return n >= min && n <= max;
}

int wrap_ft_aoti(const char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return -1;
        i++;
    }
    return (ft_atoi(str));
}

char **allocate_map_grid(int height, int width)
{
    int i;
    char **grid;

    i = 0;
    grid = ft_calloc(height + 1, sizeof(char *));
    if (!grid)
        return NULL;
    if (width > 0)
    {
        while (i < height)
        {
            grid[i] = ft_calloc(width + 1, sizeof(char));
            if (!grid[i])
            {
                while (i >= 0)
                {
                    free(grid[i]);
                    i--;
                }
                free(grid);
                return NULL;
            }
            i++;
        }
    }
    return grid;
}