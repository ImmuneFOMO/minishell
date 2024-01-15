/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_comands2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 20:34:15 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/15 00:11:15 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void handle_error(const char *error_message)
{
	write(2, error_message, ft_strlen(error_message));
	g_exit_code = 2;
}

struct s_cmd *parsepipe(char **ps, char *es, char **envp)
{
	struct s_cmd *cmd;
	cmd = parseexec(ps, es, envp);
	while (peek(ps, es, "|"))
	{
		cmd = parsepipe_errors(ps, es, cmd);
		if (cmd == NULL)
		{
			while (*ps != es)
				gettoken(ps, es, 0, 0);
			free_cmd(cmd);	
			return (NULL);
		}
		cmd = pipecmd(cmd, parseexec(ps, es, envp));
	}
	return (cmd);
}

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
		{
			parseredirs_error(ps, es);
			return (NULL);
		}
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			handle_error("syntax error near unexpected token `newline'\n");
			return (NULL);
		}
		if (tok == '<' || tok == '>' || tok == '+' || tok == '%')
			cmd = redircmd(cmd, mkcopy(q, eq), tok);
	}
	return (cmd);
}