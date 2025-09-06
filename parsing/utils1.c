#include "cub3D.h"

void check_duplicate(int *flag, int is_floor)
{
    if (*flag)
    {
        if (is_floor)
            print_err("Duplicate Floor");
        else
            print_err("Duplicate Ceiling");
    }
}