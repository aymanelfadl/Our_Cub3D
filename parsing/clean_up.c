#include "cub3D.h"


void print_err(const char *msg)
{
    printf("Error\n%s\n", msg);
    exit(EXIT_FAILURE);
}

void ft_free_split(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}