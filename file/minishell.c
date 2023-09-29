/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/09/29 02:57:39 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char *input;

	while ((input = readline("minishell> ")) != NULL)
	{
		add_history(input);
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break;
		}
	}
	return (0);
}
