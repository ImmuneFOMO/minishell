/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2023/10/23 21:56:34 by azhadan          ###   ########.fr       */
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
	else if (type == '>' || type == '<')
		redirect_command((struct s_redircmd *)cmd);
	else if (type == '|')
		pipe_command((struct s_pipecmd *)cmd);
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

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(0);
	execvp(ecmd->argv[0], ecmd->argv);
}

void	redirect_command(struct s_redircmd *rcmd)
{
	int	fd_redirect;
	int	flags;

	if (rcmd->type == '>')
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		exit(0);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		exit(0);
	}
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