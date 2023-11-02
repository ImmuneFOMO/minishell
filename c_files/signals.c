/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:51:06 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/01 18:42:14 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_c(int signum)
{
	(void)signum;
	write(1, "\nminishell: ", 12);
}

void builtin_echo(char **args)
{
    int i = 1;
    int n_flag = 0;

    if (args[i] && ft_strncmp(args[i], "-n", 3) == 0) {
        n_flag = 1;
        i++;
    }
    while (args[i]) {
        ft_printf("%s", args[i]);
        if (args[i + 1] != NULL)
            ft_printf(" ");
        i++;
    }
    if (!n_flag)
        ft_printf("\n");
}

void builtin_pwd()
{
	char dir[2000];
	
	if (getcwd(dir, sizeof(dir)) != NULL)
		ft_printf("%s\n", dir);
	else
		perror("pwd");
}

int builtins(char **argv)
{
	if (!ft_strncmp(argv[0], "pwd", 4))
	{
		builtin_pwd();
		return (1);
	}
	else if (!ft_strncmp(argv[0], "echo", 5))
    {
        builtin_echo(argv);
        return (1);
    }
	return (0);
}

char *trim_spaces(char *str) 
{
	char *end;

	while (*str == ' ' || *str == '\t')
		str++;
    if (*str == 0)
		return str;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t')) end--;
    end[1] = '\0';

    return str;
}
