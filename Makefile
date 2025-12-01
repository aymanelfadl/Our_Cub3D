CC = cc

CFLAGS = -Iincludes/ -Iminilibx-linux -Wall -Wextra -Werror

LDFLAGS = -no-pie

MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

NAME = cub3D
BONUS_NAME = cub3D_bonus

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

PARSING := $(wildcard src/parse/mandatory/*.c)
PARSING_BONUS := $(wildcard src/parse/bonus/*.c)
RAYCASTING := $(wildcard raycasting/*.c)
TEXTURES := src/textures/loader.c

<<<<<<< HEAD
PARSING = $(wildcard src/parse/mandatory/*.c)
COMMON = $(wildcard src/parse/common/*.c)

# Bonus parsing sources
PARSING_BONUS = $(wildcard src/parse/bonus/*.c)

RAYCASTING = raycasting/dda_algo.c raycasting/drawing.c raycasting/events.c \
			 raycasting/player_movement.c raycasting/ray_casting.c raycasting/utils.c
TEXTURES = src/textures/loader.c 

SRC = $(PARSING) $(COMMON) $(RAYCASTING) $(TEXTURES) main.c

# Bonus build sources
# For the bonus binary we use mandatory helpers for Error/parse_texture
# and replace core parse components with the bonus variants to avoid symbol
# duplication.
SRC_BONUS = \
	src/parse/mandatory/Error.c \
	src/parse/mandatory/parse_texture.c \
	$(COMMON) \
	$(wildcard src/parse/bonus/*.c) \
	$(RAYCASTING) $(TEXTURES) main_bonus.c

OBJ = $(SRC:.c=.o)

PARSER_OBJS = $(PARSING:.c=.o)
PARSER_TEST_OBJ = tests/parser_main.o

# Bonus parser objects
PARSER_BONUS_OBJS = src/parse/mandatory/Error.o src/parse/mandatory/parse_texture.o $(COMMON:.c=.o) $(PARSING_BONUS:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)
PARSER_BONUS_TEST_OBJ = tests/parser_bonus_test.o
=======
SRC := $(PARSING) $(RAYCASTING) $(TEXTURES) main.c
OBJ := $(SRC:.c=.o)
>>>>>>> ec24bfd (refactor(parser): update Makefile for bonus source handling and clean up cub3D.h)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@make -C minilibx-linux
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
<<<<<<< HEAD
	rm -f $(OBJ) $(OBJ_BONUS) $(PARSER_TEST_OBJ) $(PARSER_BONUS_TEST_OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) cub3D_bonus parser_tester parser_bonus_tester
=======
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)
>>>>>>> ec24bfd (refactor(parser): update Makefile for bonus source handling and clean up cub3D.h)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

# Bonus target builds separate binary that includes bonus sources
bonus: BONUS_SRC := $(SRC) $(PARSING_BONUS)
bonus: BONUS_OBJ := $(patsubst %.c,%.o,$(BONUS_SRC))
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(LIBFT)
	@make -C minilibx-linux
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(BONUS_NAME)

<<<<<<< HEAD
cub3D_bonus: $(OBJ_BONUS) $(LIBFT)
	@make -C minilibx-linux
	$(CC)  $(CFLAGS) $(OBJ_BONUS)  $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o cub3D_bonus

.PHONY: bonus
bonus: cub3D_bonus
	@echo "✅ Built cub3D_bonus"

.PHONY: all clean fclean re
=======
# Generic compile rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re bonus
>>>>>>> ec24bfd (refactor(parser): update Makefile for bonus source handling and clean up cub3D.h)
