/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:51:06 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/31 14:45:01 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_c(int signum)
{
	(void)signum;
	write(1, "\nminishell: ", 12);
}

void print_something() 
{
    printf("something\n");
}

int builtins(char *command)
{
	if (!ft_strncmp(command, "test", 4))
	{
		print_something();
		return (1);
	}
	return (0);
}
