NAME		= push_swap
BONUS_NAME	= checker

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP
INC			= -I include

SRC_DIR		= src
OBJ_DIR		= obj

CORE_SRC	= stack.c stack_rot.c ps.c output.c parse.c ranks.c utils.c

PS_SRC		= main.c strategy.c bench.c optimize.c optimize_merge.c \
			  moves.c moves_cost.c strat_simple.c lis.c strat_lis.c \
			  strat_medium.c quick.c quick_utils.c quick_small.c \
			  quick_bottom.c

BONUS_SRC	= bonus/checker_bonus.c bonus/read_ops_bonus.c

CORE_OBJ	= $(addprefix $(OBJ_DIR)/, $(CORE_SRC:.c=.o))
PS_OBJ		= $(addprefix $(OBJ_DIR)/, $(PS_SRC:.c=.o))
BONUS_OBJ	= $(addprefix $(OBJ_DIR)/, $(BONUS_SRC:.c=.o))
DEPS		= $(CORE_OBJ:.o=.d) $(PS_OBJ:.o=.d) $(BONUS_OBJ:.o=.d)

all: $(NAME)

$(NAME): $(CORE_OBJ) $(PS_OBJ)
	$(CC) $(CFLAGS) $(CORE_OBJ) $(PS_OBJ) -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(CORE_OBJ) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(CORE_OBJ) $(BONUS_OBJ) -o $(BONUS_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all bonus clean fclean re
