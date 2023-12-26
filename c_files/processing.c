/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 00:21:19 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/26 20:25:46 by root             ###   ########.fr       */
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

int	child_main(struct s_cmd *parse_cmd, char ***copy_envp, char *buf)
{
	int	code;
	int	exit_code;
	int	quote_count;
    char	*processed_buf;

	add_history(buf);
	quote_count = count_quotes(buf, '\"');
    processed_buf = handle_odd_quotes(buf, quote_count, '\"'); 
    quote_count = count_quotes(processed_buf, '\''); 
    processed_buf = handle_odd_quotes(processed_buf, quote_count, '\''); 
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
