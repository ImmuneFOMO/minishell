/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmnds.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idlbltv <idlbltv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:46 by idlbltv           #+#    #+#             */
/*   Updated: 2023/10/21 10:36:04 by idlbltv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

// int runcmd(struct cmd *cmd)
// {
// 	int fd_redirect = 0;
// 	int p_id;
// 	int fd_pipe[2];
// 	struct execcmd *ecmd;
// 	struct pipecmd *pcmd;
// 	struct redircmd *rcmd;

// 	if (cmd == 0)
// 		exit(0);

// 	char type = cmd->type;

// 	if (type == ' ')
// 	{
// 		ecmd = (struct execcmd *)cmd;
// 		if (ecmd->argv[0] == 0)
// 			exit(0);
// 		execvp(ecmd->argv[0], ecmd->argv);
// 	}
// 	else if (type == '>' || type == '<')
// 	{
// 		rcmd = (struct redircmd *)cmd;
// 		if (rcmd->type == '>')
// 		{
// 			if ((fd_redirect = open(rcmd->file, rcmd->mode, 0666)) < 0)
// 			{
// 				write(STDERR_FILENO, "open ", 5);
// 				write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
// 				write(STDERR_FILENO, " has failed\n", 11);
// 				exit(0);
// 			}
// 		}
// 		else if (rcmd->type == '<')
// 		{
// 			if ((fd_redirect = open(rcmd->file, rcmd->mode)) < 0)
// 			{
// 				write(STDERR_FILENO, "open ", 5);
// 				write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
// 				write(STDERR_FILENO, " has failed\n", 11);
// 				exit(0);
// 			}
// 		}
// 		if (dup2(fd_redirect, rcmd->fd) < 0)
// 		{
// 			write(2, "dup2 has failed\n", 15);
// 			exit(0);
// 		}
// 		runcmd(rcmd->cmd);
// 	}
// 	else if (type == '|')
// 	{
// 		pcmd = (struct pipecmd *)cmd;
// 		if (pipe(fd_pipe) < 0)
// 		{
// 			write(2, "pipe has failed\n", 14);
// 			exit(0);
// 		}
// 		if ((p_id = fork()) < 0)
// 		{
// 			write(2, "fork has failed\n", 14);
// 			exit(0);
// 		}
// 		else if (p_id == 0)
// 		{
// 			close(fd_pipe[1]);
// 			dup2(fd_pipe[0], STDIN_FILENO);
// 			runcmd(pcmd->right);
// 			close(fd_pipe[0]);
// 		}
// 		else
// 		{
// 			close(fd_pipe[0]);
// 			dup2(fd_pipe[1], STDOUT_FILENO);
// 			runcmd(pcmd->left);
// 			close(fd_pipe[1]);
// 			wait(&p_id);
// 		}
// 	}
// 	else
// 	{
// 		write(2, "unknown runcmd\n", 15);
// 		exit(1);
// 	}
// 	// exit(0);
// 	return 1;
// }

int runcmd(struct cmd *cmd)
{
	if (cmd == 0)
		exit(0);

	char type = cmd->type;

	if (type == ' ')
		executeCommand(cmd);
	else if (type == '>' || type == '<')
		redirectCommand((struct redircmd *)cmd);
	else if (type == '|')
		pipeCommand((struct pipecmd *)cmd);
	else
	{
		write(2, "unknown runcmd\n", 15);
		exit(1);
	}
	return 1;
}

void executeCommand(struct cmd *cmd)
{
	struct execcmd *ecmd = (struct execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		exit(0);
	execvp(ecmd->argv[0], ecmd->argv);
}

void redirectCommand(struct redircmd *rcmd)
{
	int fd_redirect = 0;

	if (rcmd->type == '>')
	{
		if ((fd_redirect = open(rcmd->file, rcmd->mode, 0666) < 0))
		{
			write(STDERR_FILENO, "open ", 5);
			write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
			write(STDERR_FILENO, " has failed\n", 11);
			exit(0);
		}
	}
	else if (rcmd->type == '<')
	{
		if ((fd_redirect = open(rcmd->file, rcmd->mode) < 0))
		{
			write(STDERR_FILENO, "open ", 5);
			write(STDERR_FILENO, rcmd->file, ft_strlen(rcmd->file));
			write(STDERR_FILENO, " has failed\n", 11);
			exit(0);
		}
	}
	if (dup2(fd_redirect, rcmd->fd) < 0)
	{
		write(2, "dup2 has failed\n", 15);
		exit(0);
	}
	runcmd(rcmd->cmd);
}

void pipeCommand(struct pipecmd *pcmd)
{
	int fd_pipe[2];
	int p_id;

	if (pipe(fd_pipe) < 0)
	{
		write(2, "pipe has failed\n", 14);
		exit(0);
	}

	if ((p_id = fork()) < 0)
	{
		write(2, "fork has failed\n", 14);
		exit(0);
	}
	else if (p_id == 0)
	{
		close(fd_pipe[1]);
		dup2(fd_pipe[0], STDIN_FILENO);
		runcmd(pcmd->right);
		close(fd_pipe[0]);
	}
	else
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], STDOUT_FILENO);
		runcmd(pcmd->left);
		close(fd_pipe[1]);
		wait(&p_id);
	}
}

int getcmd(char *buf, int nbuf)
{

	if (isatty(fileno(stdin)))
		write(2, "minishell> ", 11);
	ft_memset(buf, 0, nbuf);
	fgets(buf, nbuf, stdin);
	if (buf[0] == 0)
		return -1;
	return 0;
}

struct cmd *execcmd(void)
{
	struct execcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = ' ';
	return (struct cmd *)cmd;
}

struct cmd *redircmd(struct cmd *subcmd, char *file, int type)
{
	struct redircmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = type;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->mode = (type == '<') ? O_RDONLY : O_WRONLY | O_CREAT | O_TRUNC;
	cmd->fd = (type == '<') ? 0 : 1;
	return (struct cmd *)cmd;
}

struct cmd *pipecmd(struct cmd *left, struct cmd *right)
{
	struct pipecmd *cmd;

	cmd = malloc(sizeof(*cmd));
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = '|';
	cmd->left = left;
	cmd->right = right;
	return (struct cmd *)cmd;
}