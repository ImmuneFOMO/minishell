/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:51:06 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/01 13:18:00 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_c(int signum)
{
	(void)signum;
	write(1, "\nminishell: ", 12);
}

void builtin_pwd()
{
	char dir[2000];
	
	if (getcwd(dir, sizeof(dir)) != NULL)
		ft_printf("%s\n", dir);
	else
		perror("pwd");
}

int builtins(char *command)
{
	if (!ft_strncmp(command, "pwd", 4))
	{
		builtin_pwd();
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
