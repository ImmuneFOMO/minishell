/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:51:06 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/19 15:26:25 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_c(int signum)
{
	(void)signum;
	write(1, "\nminishell: ", 12);
	g_exit_code = 130;
}

void	builtin_echo(char **args)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
}

void	builtin_pwd(void)
{
	char	dir[2000];

	if (getcwd(dir, sizeof(dir)) != NULL)
		ft_printf("%s\n", dir);
	else
		perror("pwd");
}

void	builtin_env(struct s_execcmd *ecmd, char **args)
{
    int	i;

    if (args[1] != NULL)
	{
		ft_printf("fdffdfd234\n");
		ft_printf("env: %s: No such file or directory\n", args[1]);
		exit (1);
	}
    i = 0;
    if (!ecmd || !ecmd->envp)
        return;
    while (ecmd->envp[i])
    {
        ft_printf("%s\n", ecmd->envp[i]);
        i++;
    }
}

int	builtins(struct s_execcmd *ecmd)
{
	if (!ft_strncmp(ecmd->argv[0], "pwd", 4))
	{
		builtin_pwd();
		return (1);
	}
	else if (!ft_strncmp(ecmd->argv[0], "echo", 5))
	{
		builtin_echo(ecmd->argv);
		return (1);
	}
	else if (!ft_strncmp(ecmd->argv[0], "env", 4))
	{
		builtin_env(ecmd, ecmd->argv);
		return (1);
	}
	return (0);
}

char	*trim_spaces(char *str)
{
	char	*end;

	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == 0)
		return (str);
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	end[1] = '\0';
	return (str);
}
