/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 17:05:15 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/21 02:56:07 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	check_vars(char **vars)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		if (!ft_strcmp(vars[i], "|") || !ft_strcmp(vars[i], ">")
			|| !ft_strcmp(vars[i], "<") || !ft_strcmp(vars[i], "<<")
			|| !ft_strcmp(vars[i], ">>"))
			return (1);
		i++;
	}
	return (0);
}
