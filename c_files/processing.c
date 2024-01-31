/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 00:21:19 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/31 19:24:29 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*trim_spaces(char *str)
{
	char	*end;

	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == 0)
		return (str);
	end = str + ft_strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	end[1] = '\0';
	return (str);
}

char	*process_odd_q(char *buf, char quote1, char quote2)
{
	char	*temp_buf;
	int		quote_count;

	quote_count = count_quotes(buf, quote1);
	temp_buf = handle_odd_quotes(buf, quote_count, quote1);
	quote_count = count_quotes(temp_buf, quote2);
	return (handle_odd_quotes(temp_buf, quote_count, quote2));
}

char	*free_temp_buf(char *buf, char *temp_buf, char *processed_buf)
{
	if (temp_buf != buf && temp_buf != processed_buf)
		free(temp_buf);
	return (processed_buf);
}

char	*process_odd_quotes(char *buf)
{
	char	*single_quote_ptr;
	char	*double_quote_ptr;
	char	*processed_buf;

	single_quote_ptr = ft_strchr(buf, '\'');
	double_quote_ptr = ft_strchr(buf, '\"');
	if (double_quote_ptr == NULL || (single_quote_ptr != NULL \
		&& single_quote_ptr < double_quote_ptr))
		processed_buf = process_odd_q(buf, '\"', '\'');
	else
		processed_buf = process_odd_q(buf, '\'', '\"');
	return (free_temp_buf(buf, processed_buf, processed_buf));
}

int	child_main(struct s_cmd **parse_cmd, char ***copy_envp, char *buf)
{
	int		code;
	char	*processed_buf;

	add_history(buf);
	processed_buf = process_odd_quotes(buf);
	code = main_builtins(processed_buf, copy_envp);
	if (code == 2)
		return (1);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	(*parse_cmd) = parsecmd(processed_buf, (*copy_envp));
	if (processed_buf != buf)
		free(processed_buf);
	if ((*parse_cmd) == NULL)
		return (0);
	(*parse_cmd)->envp = (*copy_envp);
	runcmd((*parse_cmd));
	free_cmd((*parse_cmd));
	return (0);
}
