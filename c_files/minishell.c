/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/24 16:17:37 by azhadan          ###   ########.fr       */
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

// make a copy of the characters in 
// the input buffer, starting from s through es.
// null-terminate the copy to make it a string.
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
	int	flag;

	flag = 0;
	if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
	{
		if (chdir(buf + 3) < 0)
			write(2, "cannot cd %s\n", 13);
	}
	return (flag);
}

int	main(void)
{
	char	*buf;
	int		r;

	buf = readline("minishell> ");
	signal(SIGINT, handle_c);
	signal(SIGQUIT, SIG_IGN);
	while (buf != NULL)
	{
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
			runcmd(parsecmd(buf));
			free(buf);
			exit(0);
		}
		wait(&r);
		free(buf);
		buf = readline("minishell: ");
	}
	free(buf);
	rl_clear_history();
	return (0);
}
