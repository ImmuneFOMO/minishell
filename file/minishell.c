/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:54:21 by azhadan           #+#    #+#             */
/*   Updated: 2023/09/22 03:25:05 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	main()
{
	char *input;
	pid_t pid;
	int status;

	while (1) {
		input = readline("minishell> ");
		if (!input)
			break ;
	
	add_history(input);
	pid = fork();
	if (pid == 0)
	{
		char *args[] = {"/bin/sh", "-c", input, NULL};
		execve(args[0], args, NULL);
		perror("execve");
		exit (1);
	}
	else if (pid < 0)
		perror("fork");
	else
	{
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
	}
	free(input);
	}
	return (0);
}
