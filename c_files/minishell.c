/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/08 18:23:47 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

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

char	*mkcopy(char *s, char *es)
{
	int		n;
	char	*c;

	n = es - s;
	c = malloc(n + 1);
	if (c == NULL)
	{
		write(2, "Memory allocation failed.\n", 25);
		exit(1);
	}
	strncpy(c, s, n);
	c[n] = 0;
	return (c);
}

int	ft_cd(char *buf)
{
	char	*home_dir;

	if (ft_strncmp(buf, "cd", 4) == 0)
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			perror("ft_cd: HOME not set");
			return (1);
		}
		else
		{
			if (chdir(home_dir) < 0)
			{
				perror("ft_cd");
				return (1);
			}
		}
	}
	else if (ft_strncmp(buf, "cd ", 3) == 0)
	{
		if (chdir(buf + 3) < 0)
		{
			perror("ft_cd");
			return (1);
		}
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char			*buf;
	char			**copy_envp;
	int				r;
	struct s_cmd	*parse_cmd;
	int				code;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_c);
	(void)argc;
	(void)argv;
	copy_envp = dup_envp(envp);
	while (1)
	{
		buf = readline("minishell: ");
		if (!buf)
			break ;
		if (!buf[0])
		{
			free(buf);
			continue ;
		}
		add_history(buf);
		code = main_builtins(buf, copy_envp);
		if (code == 2)
			break ;
		else if (code == 1)
			continue ;
		if (fork1() == 0)
		{
			parse_cmd = parsecmd(buf);
			parse_cmd->envp = copy_envp;
			runcmd(parse_cmd);
			free_cmd(parse_cmd);
			exit(0);
		}
		wait(&r);
		free(buf);
	}
	free_envp(copy_envp);
	rl_clear_history();
	return (0);
}
