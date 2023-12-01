/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/01 18:25:31 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	runcmd(struct s_cmd *cmd)
{
	char	type;

	type = cmd->type;
	if (cmd == 0)
		exit(0);
	if (type == ' ')
		execute_command(cmd);
	else if (type == '>' || type == '<' || type == '+' || type == '%')
		redirect_command((struct s_redircmd *)cmd);
	else if (type == '|')
		pipe_command((struct s_pipecmd *)cmd);
	else if (type == ';')
	{
		runcmd(((struct s_semicoloncmd *)cmd)->left);
		runcmd(((struct s_semicoloncmd *)cmd)->right);
	}
	else
	{
		write(2, "unknown runcmd\n", 15);
		exit(1);
	}
	return (1);
}

void	execute_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	char				*full_path;

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(0);
	if (builtins(ecmd))
	{
		return ;
	}
	full_path = find_in_path(ecmd->argv[0]);
	if (full_path)
	{
		execve(full_path, ecmd->argv, ecmd->envp);
		free(full_path);
	}
	else
	{
		execve(ecmd->argv[0], ecmd->argv, ecmd->envp);
	}
}

void	redirect_command(struct s_redircmd *rcmd)
{
	int	fd_redirect;
	int	flags;

	if (rcmd->type == '>' || rcmd->type == '+')
		flags = rcmd->mode;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		return ;
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		exit(0);
	}
	rcmd->cmd->envp = dup_envp(rcmd->envp);
	runcmd(rcmd->cmd);
	close(fd_redirect);
}

void	pipe_command(struct s_pipecmd *pcmd)
{
	int	fd_pipe[2];

	setup_pipe(fd_pipe);
	create_pipe_process(pcmd, fd_pipe);
}

void	setup_pipe(int fd_pipe[2])
{
	if (pipe(fd_pipe) < 0)
	{
		write(2, "pipe has failed\n", 14);
		exit(0);
	}
}
