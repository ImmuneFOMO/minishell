/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/02 02:22:14 by azhadan          ###   ########.fr       */
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


#define MAXARGS 10

// All commands have at least a type. Have looked at the type, the code
// typically casts the *cmd to some specific cmd type.
struct cmd {
  int type;          //  ' ' (exec), | (pipe), '<' or '>' for redirection
};

struct execcmd {
  int type;              // ' '
  char *argv[MAXARGS];   // arguments to the command to be exec-ed
};

struct redircmd {
  int type;          // < or > 
  struct cmd *cmd;   // the command to be run (e.g., an execcmd)
  char *file;        // the input/output file
  int mode;          // the mode to open the file with
  int fd;            // the file descriptor number to use for the file
};

struct pipecmd {
  int type;          // |
  struct cmd *left;  // left side of pipe
  struct cmd *right; // right side of pipe
};

int fork1(void);  // Fork but exits on failure.
struct cmd *parsecmd(char*);
void runcmd(struct cmd *cmd);
int getcmd(char *buf, int nbuf);
struct cmd* execcmd(void);
struct cmd*	redircmd(struct cmd *subcmd, char *file, int type);
struct cmd*	pipecmd(struct cmd *left, struct cmd *right);
int	gettoken(char **ps, char *es, char **q, char **eq);
int	peek(char **ps, char *es, char *toks);
char *mkcopy(char *s, char *es);
struct cmd* parsecmd(char *s);
struct cmd* parseline(char **ps, char *es);
struct cmd* parsepipe(char **ps, char *es);
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es);
struct cmd* parseexec(char **ps, char *es);
struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);

#endif