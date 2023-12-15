/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/15 21:26:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int handle_heredoc(struct s_redircmd *rcmd)
{
    char buffer[1024];
    int pipefd[2];
    size_t delimiter_length;
    ssize_t read_len;

    if (rcmd == NULL || rcmd->file == NULL)
    {
        write(STDERR_FILENO, "Invalid command\n", 16);
        return 0;
    }
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 0;
    }
    delimiter_length = ft_strlen(rcmd->file);
    while (1)
    {
        write(STDERR_FILENO, ">", 1);
        read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (read_len <= 0)
            break;
        buffer[read_len] = '\0';
        if (ft_strncmp(buffer, rcmd->file, delimiter_length) == 0 &&
            (buffer[delimiter_length] == '\n' || buffer[delimiter_length] == '\0'))
            break;
        write(pipefd[1], buffer, read_len);
    }
    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(pipefd[0]);
        return 0;
    }
    close(pipefd[0]);
    return 1;
}

int	runcmd(struct s_cmd *cmd)
{
	char	type;
    int     exit_code = 0;

	type = cmd->type;
	if (cmd == 0)
		exit(0);
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
		exit(1);
	}
	return (exit_code);
}

int check_error(char *cmd) {
    if (errno == EACCES) {
        write(2, cmd, ft_strlen(cmd));
        write(2, ": permission denied\n", 20);
        return 126;
    } else {
        perror(cmd);
        return 127;
    }
}

int	execute_command(struct s_cmd *cmd)
{
    struct s_execcmd	*ecmd;
    char				*full_path;
    int                 exit_code = 0;

    ecmd = (struct s_execcmd *)cmd;
    if (ecmd->argv[0] == 0)
        exit(0);
    if (builtins(ecmd))
    {
        return (exit_code);
    }
    full_path = find_in_path(ecmd->argv[0]);
    if (full_path)
    {
        execve(full_path, ecmd->argv, ecmd->envp);
        if (errno)
           exit_code = check_error(ecmd->argv[0]);
        free(full_path);
    }
    else
    {
        if (ft_strncmp(ecmd->argv[0], "cd", 2) != 0 && ft_strncmp(ecmd->argv[0], "export", 6) != 0 && ft_strncmp(ecmd->argv[0], "unset", 5) != 0)
        {
            execve(ecmd->argv[0], ecmd->argv, ecmd->envp);
            if (errno)
            exit_code = check_error(ecmd->argv[0]);    
        }
    }
    return (exit_code);
}

void	redirect_command(struct s_redircmd *rcmd)
{
	int	fd_redirect;
	int	flags;

	if (rcmd->type == '>' || rcmd->type == '+')
		flags = rcmd->mode;
	else if (rcmd->type == '<')
		flags = O_RDONLY;
	else if (rcmd->type == '%')
	{
		fd_redirect = handle_heredoc(rcmd);
		if (fd_redirect == 0)
  		{
   		perror("double_redirect_left");
   		return ;
 		}
 		runcmd(rcmd->cmd);
  		return ;
	}
	fd_redirect = open(rcmd->file, flags, 0666);
	if (fd_redirect < 0)
	{
		perror("open");
		exit (1);
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
