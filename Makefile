CC = cc

CFLAGS = -Iincludes/ -Iminilibx-linux -Wall -Wextra -Werror

LDFLAGS = -no-pie

MLX_FLAGS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

NAME = cub3D
BONUS_NAME = cub3D_bonus

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Explicit mandatory parsing sources
PARSING_MANDATORY = \
	src/parse/mandatory/Error.c \
	src/parse/mandatory/parse_texture.c \
	src/parse/mandatory/valid_map.c \
	src/parse/mandatory/parser.c \
	src/parse/mandatory/parse_map.c \
	src/parse/mandatory/utils.c

# Common parse helpers
PARSE_COMMON = src/parse/common/common_utils.c

# Explicit bonus parsing sources (extra bonus-only files)
PARSING_BONUS_EXTRA = \
	src/parse/bonus/bonus_helpers.c \
	src/parse/bonus/err_bonus.c \
	src/parse/bonus/parser_bonus.c \
	src/parse/bonus/parse_map_bonus.c \
	src/parse/bonus/utils_bonus.c \
	src/parse/bonus/parser_flow_bonus.c \
	src/parse/bonus/Door_valid_bonus.c \
	src/parse/bonus/sprite_valid_bonus.c \
	src/parse/bonus/valid_map_bonus.c

# Raycasting and rendering sources
RAYCASTING = \
	raycasting/dda_algo.c \
	raycasting/drawing.c \
	raycasting/events.c \
	raycasting/player_movement.c \
	raycasting/ray_casting.c \
	raycasting/utils.c \
	raycasting/drawing_utils.c

# Raycasting bonus sources (explicit list from raycasting_bonus/)
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

# Textures loader
TEXTURES = src/textures/loader.c

# Application entry points
MAIN = main.c
MAIN_BONUS = main_bonus.c

# Mandatory build sources (explicit)
SRC = $(PARSING_MANDATORY) $(PARSE_COMMON) $(RAYCASTING) $(TEXTURES) $(MAIN)

# Bonus build sources: include mandatory Error/parse_texture helpers plus bonus parsing extras
SRC_BONUS = src/parse/mandatory/Error.c src/parse/mandatory/parse_texture.c $(PARSE_COMMON) $(PARSING_BONUS_EXTRA) $(RAYCASTING_BONUS) $(TEXTURES) $(MAIN_BONUS)

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)




$(NAME): $(OBJ) $(LIBFT)
	@make -C minilibx-linux
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(NAME)

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
	@make -C minilibx-linux
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) $(MLX_FLAGS) $(LDFLAGS) -o $(BONUS_NAME)

.PHONY: all clean fclean re bonus