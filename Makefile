CC = cc

MLX_DIR = minilibx-linux
LIBFT_DIR = libft
INCLUDES_DIR = includes

CFLAGS = -I$(INCLUDES_DIR) -I$(MLX_DIR) -Wall -Wextra -Werror
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_LIBS = -lX11 -lXext -lm
LIBFT = $(LIBFT_DIR)/libft.a

PARSING = parsing/read_file.c parsing/parse_color.c \
		  parsing/parse_texture.c parsing/clean_up.c \
		  parsing/normalize_map.c parsing/parse_map.c \
		  parsing/validate_map.c parsing/init_parsing.c \
		  parsing/utils.c parsing/utils1.c parsing/player_direction.c

RAYCASTING = raycasting/ray_casting.c

SRC = $(PARSING) $(RAYCASTING) main.c
OBJ = $(SRC:.c=.o)

NAME = cub3D

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_LIB) $(MLX_LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@make -C $(MLX_DIR)

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re