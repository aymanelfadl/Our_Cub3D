CC = cc
CFLAGS = -Iincludes/ -Iminilibx-linux -Wall -Wextra -Werror
MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm
NAME = cub3D
BONUS_NAME = cub3D_bonus
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = includes/cub3D.h

PARSING_MANDATORY = \
    src/parse/mandatory/Error.c \
    src/parse/mandatory/parse_texture.c \
    src/parse/mandatory/valid_map.c \
    src/parse/mandatory/parser.c \
    src/parse/mandatory/parse_map.c \
    src/parse/mandatory/utils.c \
    src/parse/mandatory/parser_colors.c \
    src/parse/mandatory/free.c \
    src/parse/mandatory/parse_lines.c \
    src/parse/mandatory/playerhandel.c \
    src/parse/mandatory/read_file.c \
    src/parse/mandatory/util_2.c

PARSE_COMMON = src/parse/common/common_utils.c

PARSING_BONUS_EXTRA = \
    src/parse/bonus/bonus_helpers.c \
    src/parse/bonus/err_bonus.c \
    src/parse/bonus/parser_bonus.c \
    src/parse/bonus/parse_map_bonus.c \
    src/parse/bonus/Door_valid_bonus.c \
    src/parse/bonus/valid_map_bonus.c

PARSING_BONUS_BASE = \
    src/parse/mandatory/Error.c \
    src/parse/mandatory/parse_texture.c \
    src/parse/mandatory/parser.c \
    src/parse/mandatory/utils.c \
    src/parse/mandatory/parser_colors.c \
    src/parse/mandatory/parse_lines.c \
    src/parse/mandatory/read_file.c \
    src/parse/mandatory/free.c \
    src/parse/mandatory/util_2.c \
    src/parse/mandatory/playerhandel.c \
    $(PARSE_COMMON)

RAYCASTING = \
    raycasting/dda_algo.c \
    raycasting/drawing.c \
    raycasting/events.c \
    raycasting/player_movement.c \
    raycasting/ray_casting.c \
    raycasting/utils.c \
    raycasting/drawing_utils.c

RAYCASTING_BONUS = \
    raycasting_bonus/dda_algo.c \
    raycasting_bonus/door_bonus.c \
    raycasting_bonus/drawing.c \
    raycasting_bonus/drawing_utils.c \
    raycasting_bonus/events.c \
    raycasting_bonus/player_movement.c \
    raycasting_bonus/ray_casting.c \
    raycasting_bonus/utils.c \
    raycasting_bonus/sprites_bonus.c \
    raycasting_bonus/mini_map.c \
    raycasting_bonus/movement_utils.c

TEXTURES = src/textures/loader.c

MAIN = main.c
MAIN_BONUS = main_bonus.c

SRC = $(PARSING_MANDATORY) $(PARSE_COMMON) $(RAYCASTING) $(TEXTURES) $(MAIN)

SRC_BONUS = \
    $(PARSING_BONUS_BASE) \
    $(PARSING_BONUS_EXTRA) \
    $(RAYCASTING_BONUS) \
    $(TEXTURES) \
    $(MAIN_BONUS)

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(NAME)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ) $(OBJ_BONUS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(BONUS_NAME) parser_tester parser_bonus_tester
	@make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: $(OBJ_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(BONUS_NAME)

.PHONY: all clean fclean re bonus