/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1_second.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 21:45:19 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/31 21:46:39 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	erorr_message_ex_cmnd(char *error, char *full_path)
{
	perror(error);
	free(full_path);
	exit(1);
}

void	execute_command_run2(char *full_path, struct s_execcmd *ecmd, 
				int status, int flag)
{
	if (flag == 1)
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
	if (flag == 2)
	{
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			g_exit_code = 131;
		}
	}
}

void	execute_command_run(char *full_path, struct s_execcmd *ecmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!full_path && access(ecmd->argv[0], F_OK) != 0)
	{
		g_exit_code = check_error(ecmd->argv[0]);
		free(full_path);
		return ;
	}
	pid = fork();
	if (pid < 0)
		erorr_message_ex_cmnd("fork", full_path);
	else if (pid == 0)
		execute_command_run2(full_path, ecmd, status, 1);
	else
	{
		waitpid(pid, &status, 0);
		execute_command_run2(full_path, ecmd, status, 2);
	}
	free(full_path);
}

int	execute_command(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	char				*full_path;

	ecmd = (struct s_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		return (g_exit_code);
	if (ft_strncmp(ecmd->argv[0], "cd", 3) == 0 || ft_strncmp(ecmd->argv[0],
			"export", 7) == 0 || ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
		return (g_exit_code);
	g_exit_code = 0;
	if (builtins(ecmd))
		return (g_exit_code);
	full_path = find_in_path(ecmd->argv[0]);
	execute_command_run(full_path, ecmd);
	return (g_exit_code);
}
