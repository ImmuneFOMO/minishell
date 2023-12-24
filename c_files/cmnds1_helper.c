/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 01:09:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/24 01:10:04 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	checker_handle_heredoc(struct s_redircmd *rcmd, int *pipefd)
{
	if (rcmd == NULL || rcmd->file == NULL)
	{
		write(STDERR_FILENO, "Invalid command\n", 16);
		return (1);
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

int	handle_heredoc_finish(int *pipefd)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		return (0);
	}
	close(pipefd[0]);
	return (1);
}

int	handle_heredoc(struct s_redircmd *rcmd)
{
	char	buffer[1024];
	int		pipefd[2];
	int		delimiter_length;
	ssize_t	read_len;

	if (checker_handle_heredoc(rcmd, pipefd))
	{
		return (0);
	}
	delimiter_length = ft_strlen(rcmd->file);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, rcmd->file, delimiter_length) == 0
			&& (buffer[delimiter_length] == '\n'
				|| buffer[delimiter_length] == '\0'))
			break ;
		write(pipefd[1], buffer, read_len);
	}
	return (handle_heredoc_finish(pipefd));
}

int	runcmd(struct s_cmd *cmd)
{
	char	type;
	int		exit_code;

	exit_code = 0;
	type = cmd->type;
	if (cmd == 0)
		return (0);
	if (type == ' ')
		exit_code = execute_command(cmd);
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
		return (1);
	}
	return (exit_code);
}

int	check_error(char *cmd)
{
	if (errno == EACCES)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": permission denied\n", 20);
		return (126);
	}
	else if (errno == ENOENT)
	{
		write(2, cmd, strlen(cmd));
		write(2, ": command not found\n", 20);
		return (127);
	}
	else
	{
		perror(cmd);
		return (127);
	}
}
