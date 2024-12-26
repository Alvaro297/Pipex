NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c pipex_utils.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft_pipex
LIBFT = $(LIBFT_DIR)/libft.a
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re