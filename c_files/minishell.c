/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/03 19:34:15 by idelibal         ###   ########.fr       */
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
	int		flag;
	char	*home_dir;
	char	*trimmed_buf;

	flag = 0;
	trimmed_buf = trim_spaces(buf);
	if (ft_strncmp(trimmed_buf, "cd", 4) == 0)
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			perror("ft_cd: HOME not set");
			flag = 1;
		}
		else
		{
			if (chdir(home_dir) < 0)
			{
				perror("ft_cd");
				flag = 1;
			}
		}
	}
	else if (ft_strncmp(trimmed_buf, "cd ", 3) == 0)
	{
		if (chdir(trimmed_buf + 3) < 0)
		{
			perror("ft_cd");
			flag = 1;
		}
	}
	return (flag);
}

int	main(int argc, char **argv, char **envp)
{
	char			*buf;
	int				r;
	struct s_cmd	*parse_cmd;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_c);
	(void)argc;
	(void)argv;
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
		if (ft_strncmp(buf, "exit", 4) == 0)
			break ;
		if (ft_cd(buf))
			continue ;
		if (fork1() == 0)
		{
			parse_cmd = parsecmd(buf);
			parse_cmd->envp = envp;
			runcmd(parse_cmd);
			free_cmd(parse_cmd);
			exit(0);
		}
		wait(&r);
		free(buf);
	}
	rl_clear_history();
	return (0);
}
