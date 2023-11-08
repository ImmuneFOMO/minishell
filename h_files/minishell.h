/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/08 13:34:50 by azhadan          ###   ########.fr       */
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

struct				s_cmd
{
	int				type;
	char			**envp;
};

struct				s_execcmd
{
	int				type;
	char			**envp;
	char			**argv;
};

struct				s_redircmd
{
	int				type;
	char			**envp;
	struct s_cmd	*cmd;
	char			*file;
	int				mode;
	int				fd;
};

struct				s_pipecmd
{
	int				type;
	char			**envp;
	struct s_cmd	*left;
	struct s_cmd	*right;
};

/*builtins.c 4/5*/
char 				*find_in_path(const char *cmd);
int					main_builtins(char *buf);
int 				ft_count_argc(char **ps, char *es);
char				**dup_envp(char **envp);

/*parsing.c 5/5*/
struct s_cmd		*parsecmd(char *s);
struct s_cmd		*parseline(char **ps, char *es);
struct s_cmd		*parsepipe(char **ps, char *es);
struct s_cmd		*parseredirs(struct s_cmd *cmd, char **ps, char *es);
struct s_cmd		*parseexec(char **ps, char *es);

/*cmnds2.c 5/5*/
int					getcmd(char *buf, int nbuf);
void				create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2]);
struct s_cmd		*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd		*redircmd(struct s_cmd *subcmd, char *file, int type);
struct s_cmd		*execcmd(void);

/*signals.c 5/5*/
void				handle_c(int signum);
int					builtins(struct s_execcmd *ecmd);
void 				builtin_pwd();
char				*trim_spaces(char *str);
void				builtin_env(struct s_execcmd *ecmd);


/*get_token.c 5/5*/
int					is_whitespace(char c);
int					gettoken(char **ps, char *es, char **q, char **eq);
void				process_special_tokens(char **s, int *token);
void				free_cmd(struct s_cmd *command);
void				free_envp(char **envp);


/*minishell.c 4/4*/
int					ft_cd(char *buf);
int					peek(char **ps, char *es, char *toks);
int					fork1(void);
char				*mkcopy(char *s, char *es);

/*cmnds1.c 5/5*/
int					runcmd(struct s_cmd *cmd);
void				execute_command(struct s_cmd *cmd);
void				redirect_command(struct s_redircmd *rcmd);
void				pipe_command(struct s_pipecmd *pcmd);
void				setup_pipe(int fd_pipe[2]);

#endif