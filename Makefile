NAME = minishell

CFILES = c_files/cmnds1.c c_files/cmnds2.c c_files/get_token.c c_files/minishell.c c_files/parsing.c  \
		c_files/signals.c

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
