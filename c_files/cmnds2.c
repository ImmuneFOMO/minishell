/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 20:22:16 by idelibal          #+#    #+#             */
/*   Updated: 2023/11/07 10:45:08 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2])
{
	int	p_id;

	p_id = fork();
	if (p_id < 0)
	{
		write(2, "fork has failed\n", 14);
		exit(0);
	}
	else if (p_id == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		runcmd(pcmd->right);
		close(fd_pipe[0]);
	}
	else
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		runcmd(pcmd->left);
		close(fd_pipe[1]);
		wait(&p_id);
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
	else
	{
		cmd->mode = O_WRONLY | O_CREAT | O_TRUNC;
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
