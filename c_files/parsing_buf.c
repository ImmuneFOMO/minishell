/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:50 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/31 19:28:23 by idelibal         ###   ########.fr       */
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

void	calculate_buf_change_nums(int *i, int *size, int *in_double_quotes,
		int *in_single_quotes, int flag)
{
	if (flag == 1)
	{
		(*in_double_quotes) = 0;
		(*in_single_quotes) = 0;
		(*i) = 0;
		(*size) = 0;
	}
	else if (flag == 2)
	{
		(*size)++;
		(*i)++;
	}
}

int	calculate_buf_if(int *i, int *in_double_quotes, int *in_single_quotes,
		char quote_type, char *arg)
{
	if (arg[(*i)] == '\"' && !(*in_single_quotes))
		(*in_double_quotes) = !(*in_double_quotes);
	if (arg[(*i)] == '\'' && !(*in_double_quotes))
		(*in_single_quotes) = !(*in_single_quotes);
	if (arg[(*i)] == quote_type && (!(*in_double_quotes) || quote_type == '\"')
		&& (!(*in_single_quotes) || quote_type == '\''))
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
	int		qoutes[2];

	calculate_buf_change_nums(&i, &size, &qoutes[0], &qoutes[1],
		1);
	while (arg[i] != '\0')
	{
		if (calculate_buf_if(&i, &qoutes[0], &qoutes[1],
				quote_type, arg))
			in_quotes = !in_quotes;
		else if (arg[i] == '$' && ((!in_quotes && quote_type == '\'')
				|| (in_quotes && quote_type == '\"')))
		{
			var_value = handle_env_var(arg, &i, &memory_allocated, envp);
			calculate_buf_var_val(&var_value, &size, memory_allocated);
		}
		else
			calculate_buf_change_nums(&i, &size, &qoutes[0],
				&qoutes[1], 2);
	}
	return (size + 1);
}
