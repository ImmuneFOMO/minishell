/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idlbltv           #+#    #+#             */
/*   Updated: 2023/10/26 15:08:07 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

struct s_cmd	*parsecmd(char *s)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		write(2, "leftovers: %s\n", 14);
		exit(-1);
	}
	return (cmd);
}

struct s_cmd	*parseline(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parsepipe(ps, es);
	return (cmd);
}

struct s_cmd	*parsepipe(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

struct s_cmd	*parseredirs(struct s_cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;

	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			write(2, "missing file for redirection\n", 28);
			exit(-1);
		}
		if (tok == '<' || tok == '>')
		{
			cmd = redircmd(cmd, mkcopy(q, eq), tok);
		}
	}
	return (cmd);
}

int ft_count_argc(char **ps, char *es)
{
	char				*q;
	char				*eq;
	int					tok;
	int					argc;
	struct s_cmd		*ret;

	ret = execcmd();
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			write(2, "syntax error\n", 12);
			exit(-1);
		}
		argc++;
	}
	return (argc);
}

struct s_cmd *parseexec(char **ps, char *es)
{
    char 				*q;
    char				*eq;
    int 				tok;
    int 				argc;
    struct s_execcmd 	*cmd;
    struct s_cmd 		*ret;
	char 				*ps_clone;
	char 				*es_clone;
    
    ps_clone = *ps;
    es_clone = es;
    ret = execcmd();
    cmd = (struct s_execcmd *)ret;

    ret = parseredirs(ret, ps, es);
    argc = ft_count_argc(&ps_clone, es_clone);
	cmd->argv = (char **)malloc(sizeof(char *) * argc);
	argc = 0;
    while (!peek(ps, es, "|"))
    {
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a')
        {
            write(2, "syntax error\n", 12);
            exit(-1);
        }
        cmd->argv[argc] = mkcopy(q, eq);
        argc++;
        ret = parseredirs(ret, ps, es);
    }
    cmd->argv[argc] = 0;
    return (ret);
}
