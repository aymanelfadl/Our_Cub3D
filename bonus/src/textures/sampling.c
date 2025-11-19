#include "cub3D.h"

unsigned int tex_get_pixel(t_img *img, int x, int y)
{
    if (!img || !img->addr)
        return (0);
    if (x < 0 || y < 0)
        return (0);
    char *ptr = img->addr + y * img->line_len + x * (img->bpp / 8);
    return (*(unsigned int *)ptr);
}
