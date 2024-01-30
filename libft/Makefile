NAME		= libft.a

DIR_SRCS	= flies
DIR_OBJS	= objs

SRCS		= $(wildcard $(DIR_SRCS)/*.c)
OBJS		= $(patsubst $(DIR_SRCS)/%.c, $(DIR_OBJS)/%.o, $(SRCS))

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I.
RM			= /bin/rm -f

$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.c
	@mkdir -p $(DIR_OBJS)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@ar -rcs $(NAME) $(OBJS)
	@ranlib $(NAME)

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(DIR_OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
