CC = cc

CFLAGS = -Iincludes/ -Iminilibx-linux -Wall -Wextra -Werror 

LDFLAGS = -no-pie

MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

NAME = cub3D

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a


PARSING = src/parse/parser.c \
		  src/parse/parse_map.c \
		  src/parse/parse_texture.c \
		  src/parse/utils.c \
		  src/parse/valid_map.c \
		  src/parse/Error.c

# Bonus parsing sources
PARSING_BONUS = src/parse/bonus/map_validate_bonus.c \
				src/parse/bonus/collect_doors.c \
				src/parse/bonus/collect_sprites.c \
				src/parse/bonus/parser_bonus.c

RAYCASTING = raycasting/dda_algo.c raycasting/drawing.c raycasting/events.c \
			 raycasting/player_movement.c raycasting/ray_casting.c raycasting/utils.c
TEXTURES = src/textures/loader.c 

SRC = $(PARSING) $(RAYCASTING) $(TEXTURES) main.c

OBJ = $(SRC:.c=.o)

PARSER_OBJS = $(PARSING:.c=.o)
PARSER_TEST_OBJ = tests/parser_main.o

# Bonus parser objects
PARSER_BONUS_OBJS = $(PARSING:.c=.o) $(PARSING_BONUS:.c=.o)
PARSER_BONUS_TEST_OBJ = tests/parser_bonus_test.o

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@make -C minilibx-linux
	$(CC)  $(CFLAGS) $(OBJ)  $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ) $(PARSER_TEST_OBJ) $(PARSER_BONUS_TEST_OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) parser_tester parser_bonus_tester
	@make -C $(LIBFT_DIR) fclean

re: fclean all

parser_tester: $(PARSER_OBJS) $(PARSER_TEST_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(PARSER_OBJS) $(PARSER_TEST_OBJ) $(LIBFT) -o parser_tester

# Parser bonus tester
parser_bonus_tester: $(PARSER_BONUS_OBJS) $(PARSER_BONUS_TEST_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(PARSER_BONUS_OBJS) $(PARSER_BONUS_TEST_OBJ) $(LIBFT) -o parser_bonus_tester
	@echo "✅ Parser bonus tester compiled!"

.PHONY: all clean fclean re