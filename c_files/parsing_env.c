/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:42 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/27 23:08:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	quotes_env_errors_set(int *i, int *j, int *in_double_quotes, int *in_single_quotes)
{
	(*in_double_quotes) = 0;
	(*in_single_quotes) = 0;
	(*i) = 0;
	(*j) = 0;
}

int	quotes_env_errors_exit_code(int *is_itoa, char *arg, int *i,
		char **var_value)
{
	(*is_itoa) = 0;
	if (ft_strncmp(arg + (*i), "$?", 2) == 0)
	{
		(*var_value) = ft_itoa(g_exit_code);
		(*i) += 2;
		(*is_itoa) = 1;
		return (0);
	}
	return (1);
}

int	re_e_v_ch_em(char *var_value, char **result, int *j)
{
	if (var_value != NULL)
	{
		ft_strcpy((*result) + (*j), var_value);
		(*j) += ft_strlen(var_value);
		return (1);
	}
	return (0);
}

char	*quotes_env_errors_return(char **result, int i)
{
	(*result)[i] = '\0';
	return ((*result));
}
