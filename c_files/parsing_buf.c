/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:50 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/31 19:44:05 by idelibal         ###   ########.fr       */
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

void calculate_buf_change_nums(int *params, int flag)
{
	if (flag == 1)
	{
		params[0] = 0; // i
		params[1] = 0; // size
		params[2] = 0; // in_double_quotes
		params[3] = 0; // in_single_quotes
	}
	else if (flag == 2)
	{
		params[1]++; // size
		params[0]++; // i
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

int calculate_buffer_size(char *arg, char quote_type, int in_quotes,
						  char **envp)
{
	char *var_value;
	int memory_allocated;
	int params[4];

	calculate_buf_change_nums(params, 1);
	while (arg[params[0]] != '\0')
	{
		if (calculate_buf_if(&params[0], &params[2], &params[3],
							 quote_type, arg))
			in_quotes = !in_quotes;
		else if (arg[params[0]] == '$' && ((!in_quotes && quote_type == '\'') || (in_quotes && quote_type == '\"')))
		{
			var_value = handle_env_var(arg, &params[0], &memory_allocated, envp);
			calculate_buf_var_val(&var_value, &params[1], memory_allocated);
		}
		else
			calculate_buf_change_nums(params, 2);
	}
	return (params[1] + 1);
}
