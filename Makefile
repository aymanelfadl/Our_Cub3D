CC = cc

CFLAGS = -Iincludes/ -Iminilibx-linux -Wall -Wextra -Werror 

LDFLAGS = -no-pie

MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

NAME = cub3D

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a


PARSING = src/parse/parser.c src/parse/parser_process.c \
		  src/parse/map_build.c src/parse/map_validate.c \
		  src/parse/floodfill.c src/parse/validation_general.c \
		  src/parse/validation_color.c src/parse/cleanup.c \
		  src/parse/finalize_resolution.c

RAYCASTING = raycasting/dda_algo.c raycasting/drawing.c raycasting/events.c \
			 raycasting/player_movement.c raycasting/ray_casting.c raycasting/utils.c
TEXTURES = src/textures/loader.c src/textures/sampling.c

SRC = $(PARSING) $(RAYCASTING) $(TEXTURES) main.c

OBJ = $(SRC:.c=.o)

PARSER_OBJS = $(PARSING:.c=.o)
PARSER_TEST_OBJ = tests/parser_main.o

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@make -C minilibx-linux
	$(CC)  $(CFLAGS) $(OBJ)  $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ) $(PARSER_TEST_OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) parser_tester
	@make -C $(LIBFT_DIR) fclean

re: fclean all

parser_tester: $(PARSER_OBJS) $(PARSER_TEST_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(PARSER_OBJS) $(PARSER_TEST_OBJ) $(LIBFT) -o parser_tester

.PHONY: all clean fclean re