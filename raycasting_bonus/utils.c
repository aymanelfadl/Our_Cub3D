#include "cub3D.h"

int	color_to_int(t_color c)
{
	return ((c.red << 16) | (c.green << 8) | c.blue);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return ;
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}

t_img	get_proper_texture(t_texture *texs, t_direction dir)
{
	int		i;
	t_img	rst;

	rst.addr = NULL;
	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (texs[i].id == dir)
		{
			rst = texs[i].img;
			break ;
		}
		i++;
	}
	return (rst);
}
