/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:33 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/30 22:26:50 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	count_quotes(char *arg, char quote_type)
{
	int	quote_count;
	int	i;
	int	in_double_quotes;
	int	in_single_quotes;

	in_double_quotes = 0;
	in_single_quotes = 0;
	quote_count = 0;
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (arg[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (arg[i] == quote_type && \
		(!in_double_quotes || quote_type == '\"') && \
			(!in_single_quotes || quote_type == '\''))
			quote_count++;
		i++;
	}
	return (quote_count);
}

char	*handle_odd_quotes(char *arg, int quote_count, char quote_type)
{
	char	*new_arg;
	char	extra_input[256];
	char	*temp_arg;
	ssize_t	len;

	new_arg = arg;
	while (quote_count % 2 != 0)
	{
		extra_input[0] = '\0';
		ft_printf("quote> ");
		len = read(0, extra_input, 255);
		if (extra_input[len - 1] == '\n')
			extra_input[len - 1] = '\0';
		quote_count += count_quotes(extra_input, quote_type);
		temp_arg = (char *)malloc(ft_strlen(new_arg) + ft_strlen(extra_input)
				+ 2);
		ft_strcpy(temp_arg, new_arg);
		ft_strcat(temp_arg, "\n");
		ft_strcat(temp_arg, extra_input);
		if (new_arg != arg)
			free(new_arg);
		new_arg = temp_arg;
	}
	return (new_arg);
}

char	*handle_env_var(char *arg, int *i, int *memory_allocated, char **envp)
{
	char	var_name[256];
	int		k;
	char	*env_value;

	k = 0;
	(*i)++;
	while (ft_isalnum(arg[*i]) || arg[*i] == '_')
		var_name[k++] = arg[(*i)++];
	var_name[k] = '\0';
	if (var_name[0] == '\0')
	{
		*memory_allocated = 1;
		return (ft_strdup("$"));
	}
	env_value = builtin_getenv(var_name, envp);
	if (env_value != NULL)
	{
		*memory_allocated = 1;
		return (ft_strdup(env_value));
	}
	else
	{
		*memory_allocated = 1;
		return (NULL);
	}
}

char	*process_quotes(char *arg, char quote1, char quote2, char **envp)
{
	char	*processed_arg;
	char	*temp;

	processed_arg = quotes_env_errors(arg, quote1, 0, envp);
	temp = processed_arg;
	processed_arg = quotes_env_errors(processed_arg, quote2, 0, envp);
	if (temp != arg && temp != processed_arg)
		free(temp);
	return (processed_arg);
}

char	*handle_all_quotes(char *arg, char **envp)
{
	char	*single_quote_ptr;
	char	*double_quote_ptr;

	single_quote_ptr = ft_strchr(arg, '\'');
	double_quote_ptr = ft_strchr(arg, '\"');
	if (double_quote_ptr == NULL || (single_quote_ptr != NULL \
	&& single_quote_ptr < double_quote_ptr))
		return (process_quotes(arg, '\"', '\'', envp));
	else
		return (process_quotes(arg, '\'', '\"', envp));
}
