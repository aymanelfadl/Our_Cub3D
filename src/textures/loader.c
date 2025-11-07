#include "cub3D.h"
#include <stdio.h>

int load_texture(void *mlx, t_texture *tex)
{
    int w = 0;
    int h = 0;

    if (!tex || !tex->path)
    {
        fprintf(stderr, "Error: texture or path is NULL\n");
        return (-1);
    }
    tex->img.mlx_img = mlx_xpm_file_to_image(mlx, tex->path, &w, &h);
    if (!tex->img.mlx_img)
    {
        fprintf(stderr, "Error: failed to load texture '%s'\n", tex->path);
        return (-1);
    }
    tex->img.addr = mlx_get_data_addr(tex->img.mlx_img, &tex->img.bpp, &tex->img.line_len, &tex->img.endian);
    tex->width = w;
    tex->height = h;
    tex->loaded = 1;
    return (0);
}

int texture_load_all(void *mlx, t_config *cfg)
{
    int i;

    if (!cfg)
        return (-1);
    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (cfg->textures[i].path == NULL)
        {
            fprintf(stderr, "Error: texture path for index %d is NULL\n", i);
            return (-1);
        }
        if (load_texture(mlx, &cfg->textures[i]) != 0)
        {
            /* free previously loaded textures */
            int j = 0;
            while (j < i)
            {
                if (cfg->textures[j].loaded && cfg->textures[j].img.mlx_img)
                    mlx_destroy_image(mlx, cfg->textures[j].img.mlx_img);
                cfg->textures[j].loaded = 0;
                ++j;
            }
            return (-1);
        }
        ++i;
    }
    return (0);
}

void texture_free_all(void *mlx, t_config *cfg)
{
    int i;

    if (!cfg)
        return;
    i = 0;
    while (i < TEXTURE_COUNT)
    {
        if (cfg->textures[i].loaded && cfg->textures[i].img.mlx_img)
        {
            mlx_destroy_image(mlx, cfg->textures[i].img.mlx_img);
            cfg->textures[i].img.mlx_img = NULL;
        }
        cfg->textures[i].loaded = 0;
        ++i;
    }
}
