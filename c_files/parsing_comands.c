/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_comands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:46 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/30 21:35:46 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_error(const char *error_message)
{
	write(2, error_message, ft_strlen(error_message));
	exit(2);
}

struct s_cmd	*parsecmd(char *s, char **envp)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parseline(&s, es, envp);
	peek(&s, es, "");
	if (s != es)
	{
		write(2, "leftovers: %s\n", 14);
		free(s);
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

struct s_cmd	*parseline(char **ps, char *es, char **envp)
{
	struct s_cmd	*cmd;

	cmd = parsesemicolon(ps, es, envp);
	return (cmd);
}

struct s_cmd	*parsesemicolon(char **ps, char *es, char **envp)
{
	struct s_cmd	*cmd;
	int				tok;

	cmd = parsepipe(ps, es, envp);
	if (peek(ps, es, ";"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (tok == ';' && peek(ps, es, ";"))
			handle_error("syntax error near unexpected token `;;'\n");
		cmd = semicoloncmd(cmd, parsesemicolon(ps, es, envp));
	}
	return (cmd);
}

struct s_cmd	*parsepipe(char **ps, char *es, char **envp)
{
	struct s_cmd	*cmd;
	char			*prev;

	prev = NULL;
	cmd = parseexec(ps, es, envp);
	while (peek(ps, es, "|"))
	{
		prev = *ps;
		gettoken(ps, es, 0, 0);
		while (peek(ps, es, "|"))
		{
			if (*ps != prev + 1)
				handle_error("syntax error near unexpected token `|'\n");
			else
			{
				gettoken(ps, es, 0, 0);
				if (peek(ps, es, "|"))
					handle_error("syntax error near unexpected token `|'\n");
			}
		}
		if (*ps == es)
			handle_error("syntax error near unexpected token `|'\n");
		cmd = pipecmd(cmd, parseexec(ps, es, envp));
	}
	return (cmd);
}
