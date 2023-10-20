NAME = minishell

CFILES = file/minishell.c
OFILES = $(CFILES:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror 
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OFILES)
	@make -C libft/
	@$(CC) $(CFLAGS) $(OFILES) $(LIBFT) -o $(NAME) -lreadline

clean:
	@rm -f $(OFILES)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: re, fclean, clean, all