/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/09/29 02:31:52 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h> // malloc, free, exit, getenv
#include <stdio.h>  // printf
#include <sys/wait.h> // wait, waitpid, wait3, wait4
#include <unistd.h> /* write, access, open, read, close, fork, signal, sigaction, kill,
                     getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ioctl,
                     tcsetattr, tcgetattr, ttyslot (verify for your system) */
#include <signal.h> // sigemptyset, sigaddset
#include <sys/stat.h>
#include <sys/types.h> // sys/types.h, sys/stat.h - stat, lstat, fstat, opendir, readdir, closedir
#include <string.h> // strerror, perror
#include <readline/history.h>
#include <readline/readline.h> /* readline, rl_clear_history, rl_on_new_line,
                               rl_replace_line, rl_redisplay, add_history */
#include <term.h> // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include "../libft/libft.h"

#endif