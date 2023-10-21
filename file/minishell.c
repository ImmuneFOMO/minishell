/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idlbltv <idlbltv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/20 23:35:53 by idlbltv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int fork1(void)
{
	int pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return pid;
}

int peek(char **ps, char *es, char *toks)
{
	char *s;
	char whitespace[6] = " \t\r\n\v";

	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return *s && ft_strchr(toks, *s);
}

// make a copy of the characters in the input buffer, starting from s through es.
// null-terminate the copy to make it a string.
char *mkcopy(char *s, char *es)
{
	int n = es - s;
	char *c = malloc(n + 1);
	if (c == NULL)
	{
		write(2, "Memory allocation failed.\n", 25);
		exit(1);
	}
	strncpy(c, s, n);
	c[n] = 0;
	return c;
}

int ft_cd(char *buf)
{
	int flag = 0;
	if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
	{
		if (chdir(buf + 3) < 0)
			write(2, "cannot cd %s\n", 13);
		//  free(buf);
	}
	return flag;
}

int main(void)
{
	char *buf;
	int r;
	while ((buf = readline("minishell> ")) != NULL)
	{
		if (!buf[0])
		{
			free(buf);
			continue;
		}
		add_history(buf);
		if (ft_strncmp(buf, "exit", 4) == 0)
			break;
		if (ft_cd(buf))
			continue;
		if (fork1() == 0)
		{
			runcmd(parsecmd(buf));
			free(buf);
			exit(0);
		}
		wait(&r);
		free(buf);
	}
	free(buf);
	clear_history();
	return 0;
}