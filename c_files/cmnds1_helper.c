/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 01:09:12 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/04 20:29:42 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	read_heredoc_input(const char *delimiter, int write_fd)
{
	char	buffer[1024];
	size_t	delimiter_length;
	ssize_t	read_len;

	delimiter_length = ft_strlen(delimiter);
	while (1)
	{
		write(STDERR_FILENO, ">", 1);
		read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (read_len <= 0)
			break ;
		buffer[read_len] = '\0';
		if (ft_strncmp(buffer, delimiter, delimiter_length) == 0
			&& (buffer[delimiter_length] == '\n'
				|| buffer[delimiter_length] == '\0'))
			break ;
		write(write_fd, buffer, read_len);
	}
	return (1);
}

int	double_redirect_left(struct s_redircmd *rcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (!read_heredoc_input(rcmd->file, pipefd[1]))
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		g_exit_code = 1;
		return (0);
	}
	close(pipefd[0]);
	return (1);
}

int	handle_heredoc(struct s_redircmd *rcmd)
{
	int	original_stdin;
	int	pipe_read_end;

	original_stdin = dup(STDIN_FILENO);
	pipe_read_end = double_redirect_left(rcmd);
	if (pipe_read_end < 0)
	{
		perror("double_redirect_left");
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		g_exit_code = 1;
		return (0);
	}
	runcmd(rcmd->cmd);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (1);
}

void	runcmd(struct s_cmd *cmd)
{
	char	type;

	type = cmd->type;
	if (cmd == 0)
		exit(1);
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
