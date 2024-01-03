/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2024/01/03 21:20:04 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	execute_command_run(char *full_path, struct s_execcmd *ecmd)
{
	pid_t	pid;
	int		status;

	if (!full_path && access(ecmd->argv[0], F_OK) != 0)
	{
		g_exit_code = check_error(ecmd->argv[0]);
		free(full_path);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(full_path);
		exit(1);
	}
	else if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (full_path)
		{
			execve(full_path, ecmd->argv, ecmd->envp);
			free(full_path);
		}
		else
			execve(ecmd->argv[0], ecmd->argv, ecmd->envp);
		exit(errno);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			g_exit_code = 131;
		}
	}
	free(full_path);
}

int	execute_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	char				*full_path;

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(0);
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
		exit(1);
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd_redirect);
		exit(1);
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
		exit(1);
	}
	create_pipe_process(pcmd, fd_pipe);
}
