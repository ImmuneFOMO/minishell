/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 17:05:15 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/03 12:05:03 by azhadan          ###   ########.fr       */
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

void	create_pipe_process(struct s_pipecmd *pcmd, int fd_pipe[2])
{
	int	p_id;

	p_id = fork();
	if (p_id < 0)
	{
		write(2, "fork has failed\n", 16);
		exit(1);
	}
	else if (p_id == 0)
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		close(fd_pipe[1]);
		runcmd(pcmd->left);
		free_cmd((struct s_cmd *)pcmd);
		exit(0);
	}
	else
		parent_process(fd_pipe, p_id, pcmd);
}
