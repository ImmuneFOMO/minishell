/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/03 20:10:49 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int		g_exit_code;

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
	ft_strncpy(c, s, n);
	c[n] = 0;
	return (c);
}

int	ft_cd_helper(char **envp)
{
	char	*home_dir;

	home_dir = builtin_getenv("HOME", envp);
	if (home_dir == NULL)
	{
		g_exit_code = 1;
		perror("ft_cd: HOME not set");
		return (1);
	}
	else
	{
		if (chdir(home_dir) < 0)
		{
			g_exit_code = 1;
			perror("ft_cd");
			return (1);
		}
		g_exit_code = 0;
	}
	return (0);
}

int	ft_cd(char *buf, char **envp)
{
	if (ft_strncmp(buf, "cd", 4) == 0)
	{
		if (ft_cd_helper(envp))
			return (1);
	}
	else if (ft_strncmp(buf, "cd ", 3) == 0)
	{
		if (chdir(buf + 3) < 0)
		{
			g_exit_code = 1;
			perror("ft_cd");
			return (1);
		}
		g_exit_code = 0;
	}
	return (0);
}

char	**start_main(char **argv, int argc, char ***envp,
		struct s_cmd **parse_cmd)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_c);
	(void)argc;
	(void)argv;
	g_exit_code = 0;
	(*parse_cmd) = NULL;
	return (dup_envp((*envp)));
}

int	main(int argc, char **argv, char **envp)
{
	char			*buf;
	char			**copy_envp;
	struct s_cmd	*parse_cmd;

	copy_envp = start_main(argv, argc, &envp, &parse_cmd);
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
		if (child_main(&parse_cmd, &copy_envp, buf))
			break ;
		free(buf);
	}
	free_envp(copy_envp);
	rl_clear_history();
	exit(g_exit_code);
	return (0);
}
