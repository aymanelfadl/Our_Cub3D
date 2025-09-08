CC = cc

CFLAGS = -Iincludes/  -Wall -Wextra -Werror 

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lbsd

NAME = cub3D

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a


PARSING = parsing/read_file.c parsing/parse_color.c \
		  parsing/parse_texture.c parsing/clean_up.c \
		  parsing/normalize_map.c parsing/parse_map.c \
		  parsing/validate_map.c parsing/init_parsing.c \
		  parsing/utils.c parsing/utils1.c parsing/player_direction.c

RAYCASTING = raycasting/ray_casting.c

SRC = $(PARSING) $(RAYCASTING) main.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) 
	$(CC)  $(CFLAGS) $(OBJ)  $(LIBFT) $(MLX_FLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re