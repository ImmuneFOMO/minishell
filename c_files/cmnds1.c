/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/31 21:46:18 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

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
			perror("Heredoc");
			return (1);
		}
		return (1);
	}
	return (0);
}

void	erorr_message(char *error)
{
	perror(error);
	exit(1);
}

void	close_fd(char *error, int fd_redirect, int saved_fd, int flag)
{
	if (flag == 1)
	{
		perror(error);
		close(fd_redirect);
		exit(1);
	}
	else if (flag == 2)
	{
		perror(error);
		close(fd_redirect);
		close(saved_fd);
		exit(1);
	}
}

void	redirect_command(struct s_redircmd *rcmd)
{
	int	fd_redirect;
	int	saved_fd;
	int	flags;

	if (checkes_redirect_command(&flags, rcmd, &fd_redirect))
		return ;
	saved_fd = dup(rcmd->fd);
	if (saved_fd < 0)
		erorr_message("dup");
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
		close_fd("open", fd_redirect, saved_fd, 1);
	if (dup2(fd_redirect, rcmd->fd) < 0)
		close_fd("dup2", fd_redirect, saved_fd, 2);
	runcmd(rcmd->cmd);
	close(fd_redirect);
	if (dup2(saved_fd, rcmd->fd) < 0)
		erorr_message("dup2 restore");
	close(saved_fd);
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
