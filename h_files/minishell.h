/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/24 17:31:48 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
/* malloc, free, exit, getenv */
# include <stdio.h>
/* printf */
# include <sys/wait.h>
/* wait, waitpid, wait3, wait4 */
# include <unistd.h>
/* write, access, open, read, close, fork, signal, sigaction, kill,
getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ioctl,
tcsetattr, tcgetattr, ttyslot (verify for your system) */
# include <signal.h>
/* sigemptyset, sigaddset */
# include <sys/stat.h>
# include <sys/types.h>
/*sys/types.h, sys/stat.h - stat, lstat, fstat, opendir, readdir, closedir*/
# include <string.h>
/* strerror, perror */
# include <readline/history.h>
# include <readline/readline.h>
/* readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history */
# include <term.h>
/* tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs */
# include "../libft/libft.h"

# define MAXARGS 10

struct				s_cmd
{
	int				type;
};

struct				s_execcmd
{
	int				type;
	char			*argv[MAXARGS];
};

struct				s_redircmd
{
	int				type;
	struct s_cmd	*cmd;
	char			*file;
	int				mode;
	int				fd;
};

struct				s_pipecmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
};

struct s_cmd		*execcmd(void);
/*cmnds2.c*/
struct s_cmd		*redircmd(struct s_cmd *subcmd, char *file, int type);
/*cmnds2.c*/
struct s_cmd		*pipecmd(struct s_cmd *left, struct s_cmd *right);
/*cmnds2.c*/
struct s_cmd		*parsecmd(char *s);
/*parsing.c*/
struct s_cmd		*parseline(char **ps, char *es);
/*parsing.c*/
struct s_cmd		*parsepipe(char **ps, char *es);
/*parsing.c*/
struct s_cmd		*parseredirs(struct s_cmd *cmd, char **ps, char *es);
/*parsing.c*/
struct s_cmd		*parseexec(char **ps, char *es);
/*parsing.c*/

char				*mkcopy(char *s, char *es);
/*minishell.c*/

void				execute_command(struct s_cmd *cmd);
/*cmnds1.c*/
void				redirect_command(struct s_redircmd *rcmd);
/*cmnds1.c*/
void				pipe_command(struct s_pipecmd *pcmd);
/*cmnds1.c*/
void				setup_pipe(int fd_pipe[2]);
/*cmnds1.c*/
void				create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2]);
/*cmnds2.c*/
void				handle_c(int signum);
/*signals.c*/
void				skip_non_special_tokens(char **s, char *es);
/*get_token.c*/
void				process_special_tokens(char **s, int *token);
/*get_token.c*/

int					fork1(void);
/*minishell.c*/
int					runcmd(struct s_cmd *cmd);
/*cmnds1.c*/
int					getcmd(char *buf, int nbuf);
/*cmnds2.c*/
int					gettoken(char **ps, char *es, char **q, char **eq);
/*get_token.c*/
int					peek(char **ps, char *es, char *toks);
/*minishell.c*/
int					is_whitespace(char c);
/*get_token.c*/
int					ft_cd(char *buf);
/*minishell.c*/

#endif