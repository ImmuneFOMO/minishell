/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:50 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/24 00:07:45 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	calculate_buf_var_val(char **var_value, int *size, int memory_allocated)
{
	if ((*var_value) != NULL)
	{
		(*size) += ft_strlen((*var_value));
		if (memory_allocated)
			free((*var_value));
	}
}

int	calculate_buf_var_code_error(char **arg, int *i, int *size)
{
	if (ft_strncmp((*arg) + (*i), "$?", 2) == 0)
	{
		(*size) += 3;
		(*i) += 2;
		return (0);
	}
	return (1);
}

void	calculate_buf_change_nums(int *i, int *size, int *in_double_quotes,
		int flag)
{
	if (flag == 1)
	{
		(*in_double_quotes) = 0;
		(*i) = 0;
		(*size) = 0;
	}
	else if (flag == 2)
	{
		(*size)++;
		(*i)++;
	}
}

int	calculate_buf_if(int *i, int *in_double_quotes, char quote_type, char *arg)
{
	if (arg[(*i)] == '\"')
		(*in_double_quotes) = !(*in_double_quotes);
	if (arg[(*i)] == quote_type && (!(*in_double_quotes) || \
	quote_type == '\"'))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

int	calculate_buffer_size(char *arg, char quote_type, int in_quotes,
		char **envp)
{
	char	*var_value;
	int		i;
	int		size;
	int		memory_allocated;
	int		in_double_quotes;

	calculate_buf_change_nums(&i, &size, &in_double_quotes, 1);
	while (arg[i] != '\0')
	{
		if (calculate_buf_if(&i, &in_double_quotes, quote_type, arg))
			in_quotes = !in_quotes;
		else if (arg[i] == '$' && ((!in_quotes && quote_type == '\'')
				|| (in_quotes && quote_type == '\"'))
			&& calculate_buf_var_code_error(&arg, &i, &size))
		{
			var_value = handle_env_var(arg, &i, &memory_allocated, envp);
			calculate_buf_var_val(&var_value, &size, memory_allocated);
		}
		else
			calculate_buf_change_nums(&i, &size, &in_double_quotes, 2);
	}
	return (size + 1);
}
