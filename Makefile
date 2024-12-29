NAME = pipex
BONUS_NAME = pipex_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c pipex_utils.c
SRCB = main_bonus.c pipex_utils_bonus.c here_doc.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(SRCB:.c=.o)
LIBFT_DIR = libft_pipex
LIBFT = $(LIBFT_DIR)/libft.a
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

bonus: $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus