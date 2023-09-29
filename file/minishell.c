/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/09/29 04:21:35 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char *input;

	(void)argc;
	(void)argv;
	(void)envp;
	while ((input = readline("minishell> ")) != NULL)
	{
		if (!input[0])
			continue;
		add_history(input);
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break;
		}
	}
	return (0);
}
