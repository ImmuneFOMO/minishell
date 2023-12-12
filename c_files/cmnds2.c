/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 20:22:16 by idelibal          #+#    #+#             */
/*   Updated: 2023/12/12 23:05:09 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2]) {
    int p_id;
    int status;

    p_id = fork();
    if (p_id < 0) 
	{
        write(2, "fork has failed\n", 16);
        exit(1);
    } 
	else if (p_id == 0) 
	{
		signal(SIGINT, handle_c); // add this line
        close(fd_pipe[0]);
        dup2(fd_pipe[1], STDOUT_FILENO);
        close(fd_pipe[1]);
        runcmd(pcmd->left);
		free_cmd((struct s_cmd *)pcmd);
        exit(g_exit_code);
    } 
	else 
	{
        close(fd_pipe[1]);
        dup2(fd_pipe[0], STDIN_FILENO);
        close(fd_pipe[0]);
        waitpid(p_id, &status, 0);
        if (WIFSIGNALED(status))
            g_exit_code = 127 + WTERMSIG(status);
        else if (WIFEXITED(status))
            g_exit_code = WEXITSTATUS(status);
		pcmd->right->envp = dup_envp(pcmd->envp);
        runcmd(pcmd->right);
    }
}

int	getcmd(char *buf, int nbuf)
{
	if (isatty(fileno(stdin)))
		write(2, "minishell: ", 11);
	ft_memset(buf, 0, nbuf);
	fgets(buf, nbuf, stdin);
	if (buf[0] == 0)
		return (-1);
	return (0);
}

struct s_cmd	*execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ' ';
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*redircmd(struct s_cmd *subcmd, char *file, int type)
{
	struct s_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->cmd = subcmd;
	cmd->file = file;
	if (type == '<')
	{
		cmd->mode = O_RDONLY;
		cmd->fd = 0;
	}
	else if (type == '>')
	{
		cmd->mode = O_WRONLY | O_CREAT | O_TRUNC;
		cmd->fd = 1;
	}
	else if (type == '+')
	{
		cmd->mode = O_WRONLY | O_CREAT | O_APPEND;
		cmd->fd = 1;
	}
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*pipecmd(struct s_cmd *left, struct s_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = '|';
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}

struct s_cmd *semicoloncmd(struct s_cmd *left, struct s_cmd *right)
{
    struct s_semicoloncmd *cmd;

    cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
    cmd->type = ';';
    cmd->left = left;
    cmd->right = right;
    return (struct s_cmd *)cmd;
}
