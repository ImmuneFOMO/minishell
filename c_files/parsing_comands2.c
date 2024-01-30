/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_comands2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 20:34:15 by root              #+#    #+#             */
/*   Updated: 2024/01/30 21:40:13 by azhadan          ###   ########.fr       */
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
