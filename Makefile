NAME = minishell

CFILES = c_files/cmnds1.c c_files/cmnds2.c c_files/get_token.c c_files/minishell.c c_files/parsing.c  \
		c_files/signals.c c_files/builtins.c c_files/builtins_second.c c_files/help_function.c \
		c_files/builtins_helper.c c_files/builtins_in_main.c c_files/builtins_second_helper.c \
		c_files/builtins_saver.c c_files/parsing_buf.c c_files/parsing_env.c c_files/parsing_comands.c \
		c_files/parsing_quotes.c c_files/processing.c c_files/cmnds1_helper.c

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

test:
	make re
	clear
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp -s ./minishell

tests: $(NAME)
	@./test_minishell.sh

.PHONY: re, fclean, clean, all test
