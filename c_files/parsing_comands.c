/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_comands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:51:46 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/15 00:11:11 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

struct s_cmd	*parsecmd(char *s, char **envp)
{
	char			*es;
	struct s_cmd 	*cmd;

	es = s + ft_strlen(s);
	cmd = parseline(&s, es, envp);
	peek(&s, es, "");
	if (s != es)
	{
		write(2, "leftovers: %s\n", 14);
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}

struct s_cmd	*parseline(char **ps, char *es, char **envp)
{
	struct s_cmd *cmd;

	cmd = parsesemicolon(ps, es, envp);
	return (cmd);
}

struct s_cmd	*parsesemicolon(char **ps, char *es, char **envp)
{
	struct s_cmd	*cmd;
	int 			tok;

	cmd = parsepipe(ps, es, envp);
	if (peek(ps, es, ";"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (tok == ';' && peek(ps, es, ";"))
		{
			handle_error("syntax error near unexpected token `;;'\n");
			free_cmd(cmd);
			while (*ps != es && **ps == ';')
				gettoken(ps, es, 0, 0);
			return (NULL);	
		}
		cmd = semicoloncmd(cmd, parsesemicolon(ps, es, envp));
	}
	return (cmd);
}

struct s_cmd *handle_pipe_errors(char **ps, char *es, struct s_cmd *cmd)
{
	handle_error("syntax error near unexpected token `|'\n");
	free_cmd(cmd);
	while (*ps != es)
		gettoken(ps, es, 0, 0);
	return (NULL);
}

struct s_cmd *parsepipe_errors(char **ps, char *es, struct s_cmd *cmd)
{
	char *prev;
	prev = *ps;
	gettoken(ps, es, 0, 0);
	while (peek(ps, es, "|"))
	{
		if (*ps != prev + 1)
			return handle_pipe_errors(ps, es, cmd);
		else
		{
			gettoken(ps, es, 0, 0);
			if (peek(ps, es, "|"))
				return handle_pipe_errors(ps, es, cmd);
		}
	}
	if (*ps == es)
		return handle_pipe_errors(ps, es, cmd);
	return (cmd);
}
