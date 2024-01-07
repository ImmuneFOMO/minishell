/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 17:05:15 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/05 19:27:57 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	check_vars(char **vars)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		if (!ft_strcmp(vars[i], "|") || !ft_strcmp(vars[i], ">")
			|| !ft_strcmp(vars[i], "<") || !ft_strcmp(vars[i], "<<")
			|| !ft_strcmp(vars[i], ">>"))
			return (1);
		i++;
	}
	return (0);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return (pid);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && ft_strchr(" \t\r\n\v", *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

void	parent_process(int fd_pipe[2], int p_id, struct s_pipecmd *pcmd)
{
	int	status;

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

pid_t	execute_left_command(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (left_pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		runcmd(pcmd->left);
		if (pcmd->envp)
			ft_free_strs(pcmd->envp);
		free_cmd((struct s_cmd *)pcmd);
		exit(EXIT_SUCCESS);
	}
	return (left_pid);
}

pid_t	execute_right_command(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (right_pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		runcmd(pcmd->right);
		if (pcmd->envp)
			ft_free_strs(pcmd->envp);
		free_cmd((struct s_cmd *)pcmd);
		exit(EXIT_SUCCESS);
	}
	return (right_pid);
}

void	create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2])
{
	pid_t	left_pid;
	pid_t	right_pid;

	left_pid = execute_left_command(pcmd, fd_pipe);
	if (pcmd->left->type == '-')
		waitpid(left_pid, NULL, 0);
	right_pid = execute_right_command(pcmd, fd_pipe);
	if (pcmd->right->type == '-')
		waitpid(right_pid, NULL, 0);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}
