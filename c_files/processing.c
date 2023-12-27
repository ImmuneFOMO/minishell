/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 00:21:19 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/27 22:52:32 by root             ###   ########.fr       */
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
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	end[1] = '\0';
	return (str);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

void	process_special_tokens(char **s, int *token)
{
	if (*token == '|')
		(*s)++;
	else if (*token == ';')
		(*s)++;
	else if (*token == '>')
	{
		(*s)++;
		if (**s == '>')
		{
			*token = '+';
			(*s)++;
		}
	}
	else if (*token == '<')
	{
		(*s)++;
		if (**s == '<')
		{
			*token = '%';
			(*s)++;
		}
	}
	else if (*token != '\0')
		*token = 'a';
}

char *process_odd_quotes(char *buf)
{
    char *single_quote_ptr;
    char *double_quote_ptr;
    char *processed_buf;
    char *temp_buf;
	int quote_count;

    single_quote_ptr = strchr(buf, '\'');
    double_quote_ptr = strchr(buf, '\"');
    if (double_quote_ptr == NULL || (single_quote_ptr != NULL && single_quote_ptr < double_quote_ptr))
    {
        quote_count = count_quotes(buf, '\"');
        temp_buf = handle_odd_quotes(buf, quote_count, '\"');
        quote_count = count_quotes(temp_buf, '\'');
        processed_buf = handle_odd_quotes(temp_buf, quote_count, '\'');
    }
    else
    {
        quote_count = count_quotes(buf, '\'');
        temp_buf = handle_odd_quotes(buf, quote_count, '\'');
        quote_count = count_quotes(temp_buf, '\"');
        processed_buf = handle_odd_quotes(temp_buf, quote_count, '\"');
    }

    if (temp_buf != buf && temp_buf != processed_buf)
        free(temp_buf);

    return (processed_buf);
}

int	child_main(struct s_cmd *parse_cmd, char ***copy_envp, char *buf)
{
	int	code;
	int	exit_code;
    char	*processed_buf;

	add_history(buf);
    processed_buf = process_odd_quotes(buf); 
	code = main_builtins(processed_buf, copy_envp);
	if (code == 2)
		return (1);
	if (fork1() == 0)
	{
		parse_cmd = parsecmd(processed_buf, (*copy_envp));
		parse_cmd->envp = (*copy_envp);
		exit_code = runcmd(parse_cmd);
		free_cmd(parse_cmd);
		exit(exit_code);
	}
	if (processed_buf != buf)
		free(processed_buf);
	return (0);
}

void	finish_child_main(int r, char **buf)
{
	if (WIFSIGNALED(r))
		g_exit_code = 127 + WTERMSIG(r);
	else if (WIFEXITED(r))
		g_exit_code = WEXITSTATUS(r);
	free((*buf));
}
