/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/24 01:09:58 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	execute_command_run(char *full_path, struct s_execcmd *ecmd)
{
	if (full_path)
	{
		execve(full_path, ecmd->argv, ecmd->envp);
		if (errno)
			g_exit_code = check_error(ecmd->argv[0]);
		free(full_path);
	}
	else
	{
		if (ft_strncmp(ecmd->argv[0], "cd", 3) != 0 && ft_strncmp(ecmd->argv[0],
				"export", 7) != 0 && ft_strncmp(ecmd->argv[0], "unset", 6) != 0)
		{
			execve(ecmd->argv[0], ecmd->argv, ecmd->envp);
			if (errno)
				g_exit_code = check_error(ecmd->argv[0]);
		}
	}
}

int	execute_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	char				*full_path;

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		return (0);
	if (ft_strncmp(ecmd->argv[0], "cd", 3) == 0 || ft_strncmp(ecmd->argv[0],
			"export", 7) == 0 || ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
		return (g_exit_code);
	if (builtins(ecmd))
		return (g_exit_code);
	full_path = find_in_path(ecmd->argv[0]);
	execute_command_run(full_path, ecmd);
	return (g_exit_code);
}

int	checkes_redirect_command(int *flags, struct s_redircmd *rcmd,
		int *fd_redirect)
{
	if (rcmd->type == '>' || rcmd->type == '+')
		(*flags) = rcmd->mode;
	else if (rcmd->type == '<')
		(*flags) = O_RDONLY;
	else if (rcmd->type == '%')
	{
		(*fd_redirect) = handle_heredoc(rcmd);
		if ((*fd_redirect) == 0)
		{
			perror("double_redirect_left");
			return (1);
		}
		runcmd(rcmd->cmd);
		return (1);
	}
	return (0);
}

void	redirect_command(struct s_redircmd *rcmd)
{
	int	fd_redirect;
	int	flags;

	if (checkes_redirect_command(&flags, rcmd, &fd_redirect))
		return ;
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		close(fd_redirect);
		return ;
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		return ;
	}
	rcmd->cmd->envp = dup_envp(rcmd->envp);
	runcmd(rcmd->cmd);
	close(fd_redirect);
}

void	pipe_command(struct s_pipecmd *pcmd)
{
	int	fd_pipe[2];

	if (pipe(fd_pipe) < 0)
	{
		write(2, "pipe has failed\n", 14);
		return ;
	}
	create_pipe_process(pcmd, fd_pipe);
}
