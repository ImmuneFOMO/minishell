/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/09/21 22:24:56 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
/* libft - ft_atoi, ft_bzero, ft_calloc, ft_isalnum, ft_isalpha, ft_isascii,
	ft_isdigit, ft_isprint, ft_itoa, ft_lstadd_back, ft_lstadd_front,
	ft_lstclear, ft_lstdelone, ft_lstiter, ft_lstlast, ft_lstmap, ft_lstnew,
	ft_lstsize, ft_memchr, ft_memcmp, ft_memcpy, ft_memmove, ft_memset,
	ft_printf, ft_putchar_fd, ft_putendl_fd, ft_putnbr_fd, ft_putstr_fd,
	ft_source, ft_split, ft_strchr, ft_strdup, ft_striteri,
	ft_strjoin, ft_strlcat, ft_strlcpy, ft_strlen, ft_strmapi, ft_strncmp,
	ft_strnstr, ft_strrchr, ft_strtrim, ft_substr, ft_tolower, ft_toupper,
	get_next_line */
# include <stdlib.h>
// stdlib.h - malloc, free, exit, getenv
# include <sys/wait.h>
// sys/wait.h - wait, waitpid, wait3, wait4
# include <unistd.h>
// unistd.h - write, access, open, read, close, fork, signal, sigaction, kill,
// getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ioctl,
//	tcsetattr, tcgetattr
# include <signal.h>
// signal.h - sigemptyset, sigaddset
# include <sys/stat.h>
# include <sys/types.h>
// sys/types.h, sys/stat.h - stat, lstat, fstat, opendir, readdir, closedir
# include <string.h>
// string.h - strerror, perror
# include <readline/history.h>
# include <readline/readline.h>
// readline/readline.h, readline/history.h - readline, rl_clear_history,
//	rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history


#endif