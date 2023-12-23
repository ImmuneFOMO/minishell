/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_saver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:41:58 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/23 23:44:29 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*builtin_getenv(const char *var, char **envp)
{
	int	i;

	if (var == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		if (!strncmp(envp[i], var, strlen(var)) && envp[i][strlen(var)] == '=')
		{
			return (&envp[i][strlen(var) + 1]);
		}
		i++;
	}
	return (NULL);
}

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}
