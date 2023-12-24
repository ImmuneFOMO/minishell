/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:33 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/23 23:56:16 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	parseredirs_error(char **ps, char *es)
{
	int	next_tok;

	next_tok = gettoken(ps, es, 0, 0);
	if (next_tok == '>')
		handle_error("syntax error near unexpected token `>'\n");
	else
		handle_error("syntax error near unexpected token `>>'\n");
}

struct s_cmd	*parseredirs(struct s_cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;

	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (peek(ps, es, ">"))
			parseredirs_error(ps, es);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			write(2, "missing file or redirection\n", 29);
			g_exit_code = 2;
			return (cmd);
		}
		if (tok == '<' || tok == '>' || tok == '+' || tok == '%')
			cmd = redircmd(cmd, mkcopy(q, eq), tok);
	}
	return (cmd);
}

int	count_quotes(char *arg, char quote_type)
{
	int	quote_count;
	int	i;
	int	in_double_quotes;

	in_double_quotes = 0;
	quote_count = 0;
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '\"')
		{
			in_double_quotes = !in_double_quotes;
		}
		if (arg[i] == quote_type && (!in_double_quotes || quote_type == '\"'))
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
