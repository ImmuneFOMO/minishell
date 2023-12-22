/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:26:57 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/22 21:39:15 by azhadan          ###   ########.fr       */
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
/* errno */
# include <errno.h>

extern int			g_exit_code;

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

struct				s_semicoloncmd
{
	int				type;
	char			**envp;
	struct s_cmd	*left;
	struct s_cmd	*right;
};

/*builtins.c 5/16*/
int					ft_count_argc(char **ps, char *es);
int					is_valid_number(char *str);
void				builtin_exit_errors(int argc, char ***args);
int					builtin_exit(char *buf);
void				clean_builtin(char ***vars, char **processed_var);
int					builtin_processing_unset(char **processed_var,
						char ***envp);
int					builtin_processing_export(char **processed_var,
						char ***envp, char **var, int i);
int					builtin_check_unset_export(char **processed_var,
						char ***envp);
int					main_builtins_check(char **processed_var, char ***envp);
int					main_builtins(char *buf, char ***envp);
char				**dup_envp(char **envp);
char				*create_full_path(char *dir, char *command);
char				*search_command_in_dirs(char *temp, char *command);
char				*find_in_path(char *cmd);
void				builtin_unset(char *var, char ***envp);
char				*builtin_getenv(const char *var, char **envp);

/*builtns_second 5/11*/
int					is_valid_var_name(char *var_name);
int					builtin_export_name_valid(int var_name_length, char **var,
						char ***var_val);
int					check_for_equal_sign(char *var);
int					update_or_add_env_var(char ***envp, char ***var_val);
char				*allocation_update_env_variable(char ***envp, int index,
						char ***var_val);
int					update_environment_variable(char ***envp, char ***var_val,
						int index, char ***env_var_val);
int					add_new_env_var_finish(char **new_val, char ***var_val,
						char ***new_envp, int size);
char				**add_new_new_envp(int size, char ***envp, char ***var_val);
int					add_new_environment_variable(char ***envp, char ***var_val,
						int size);
int					builtin_export(char *var, char ***envp);
void				ft_free_strs(char **strs);

/*parsing.c 5/12*/
struct s_cmd		*parsecmd(char *s, char **envp);
struct s_cmd		*parseline(char **ps, char *es, char **envp);
struct s_cmd		*parsepipe(char **ps, char *es, char **envp);
struct s_cmd		*parseredirs(struct s_cmd *cmd, char **ps, char *es);
struct s_cmd		*parseexec(char **ps, char *es, char **envp);
struct s_cmd		*parsesemicolon(char **ps, char *es, char **envp);
int					count_quotes(char *arg, char quote_type);
char				*handle_odd_quotes(char *arg, int quote_count,
						char quote_type);
char				*handle_env_var(char *arg, int *i, int *memory_allocated,
						char **envp);
int					calculate_buffer_size(char *arg, char quote_type,
						int in_quotes, char **envp);
char				*replace_env_vars(char *arg, char quote_type, int in_quotes,
						char **envp);
char				*handle_quotes(char *arg, char quote_type, char **envp);
void				parseredirs_error(char **ps, char *es);

/*cmnds2.c 5/6*/
int					getcmd(char *buf, int nbuf);
void				create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2]);
struct s_cmd		*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd		*redircmd(struct s_cmd *subcmd, char *file, int type);
struct s_cmd		*execcmd(void);
struct s_cmd		*semicoloncmd(struct s_cmd *left, struct s_cmd *right);

/*signals.c 5/5*/
void				handle_c(int signum);
int					builtins(struct s_execcmd *ecmd);
void				builtin_pwd(void);
char				*trim_spaces(char *str);
void				builtin_env(struct s_execcmd *ecmd, char **args);

/*get_token.c 5/5*/
int					is_whitespace(char c);
int					gettoken(char **ps, char *es, char **q, char **eq);
void				process_special_tokens(char **s, int *token);
void				free_cmd(struct s_cmd *command);
void				free_envp(char **envp);

/*minishell.c 4/4*/
int					ft_cd(char *buf, char **envp);
int					peek(char **ps, char *es, char *toks);
int					fork1(void);
int					ft_cd_helper(char **envp);
char				*mkcopy(char *s, char *es);

/*cmnds1.c 5/5*/
int					checker_handle_heredoc(struct s_redircmd *rcmd,
						int *pipefd);
int					handle_heredoc_finish(int *pipefd);
int					handle_heredoc(struct s_redircmd *rcmd);
int					runcmd(struct s_cmd *cmd);
void				pipe_command(struct s_pipecmd *pcmd);
int					check_error(char *cmd);
void				execute_command_run(char *full_path,
						struct s_execcmd *ecmd);
int					execute_command(struct s_cmd *cmd);
void				redirect_command(struct s_redircmd *rcmd);

/*help_function.c 5/1*/
int					check_vars(char **vars);

#endif