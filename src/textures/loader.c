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
    if (!mlx)
    {
        fprintf(stderr, "Error: mlx is NULL when loading texture '%s'\n", tex->path);
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
    tex->img.width = w;
    tex->img.height = h;
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
    /* Load optional sprite texture if provided */
    if (cfg->sprite_path)
    {
        cfg->sprite_texture.path = cfg->sprite_path;
        if (load_texture(mlx, &cfg->sprite_texture) != 0)
        {
            /* free already loaded textures */
            int j = 0;
            while (j < TEXTURE_COUNT)
            {
                if (cfg->textures[j].loaded && cfg->textures[j].img.mlx_img)
                    mlx_destroy_image(mlx, cfg->textures[j].img.mlx_img);
                cfg->textures[j].loaded = 0;
                ++j;
            }
            return (-1);
        }
    }
    /* Load optional door texture if provided */
    if (cfg->door_texture_path)
    {
        cfg->door_texture.path = cfg->door_texture_path;
        if (load_texture(mlx, &cfg->door_texture) != 0)
        {
            int j = 0;
            while (j < TEXTURE_COUNT)
            {
                if (cfg->textures[j].loaded && cfg->textures[j].img.mlx_img)
                    mlx_destroy_image(mlx, cfg->textures[j].img.mlx_img);
                cfg->textures[j].loaded = 0;
                ++j;
            }
            if (cfg->sprite_texture.loaded && cfg->sprite_texture.img.mlx_img)
                mlx_destroy_image(mlx, cfg->sprite_texture.img.mlx_img);
            cfg->sprite_texture.loaded = 0;
            return (-1);
        }
    }
    return (0);
}

int load_sprite_frames(void *mlx, t_game *game)
{
    char *sprite_paths[4];
    int i;
    int w, h;
    
    sprite_paths[0] = "textures/sprite_1.xpm";
    sprite_paths[1] = "textures/sprite_2.xpm";
    sprite_paths[2] = "textures/sprite_3.xpm";
    sprite_paths[3] = "textures/eagle.xpm"; // Reuse first frame for 4th
    
    i = 0;
    while (i < 4)
    {
        game->sprite_textures[i].mlx_img = mlx_xpm_file_to_image(mlx, sprite_paths[i], &w, &h);
        if (!game->sprite_textures[i].mlx_img)
        {
            fprintf(stderr, "Error: failed to load sprite frame '%s'\n", sprite_paths[i]);
            return (-1);
        }
        game->sprite_textures[i].addr = mlx_get_data_addr(
            game->sprite_textures[i].mlx_img,
            &game->sprite_textures[i].bpp,
            &game->sprite_textures[i].line_len,
            &game->sprite_textures[i].endian
        );
        game->sprite_textures[i].width = w;
        game->sprite_textures[i].height = h;
        i++;
    }
    game->sprite_frame_count = 4;
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
