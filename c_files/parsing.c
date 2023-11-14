/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idlbltv           #+#    #+#             */
/*   Updated: 2023/11/08 13:35:58 by azhadan          ###   ########.fr       */
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

int count_quotes(char *arg, char quote_type)
{
    int	quote_count;
    int	i;
    quote_count = 0;
    i = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == quote_type)
            quote_count++;
        i++;
    }
    return (quote_count);
}

char *handle_odd_quotes(char *arg, int quote_count, char quote_type)
{
    char *new_arg;
    new_arg = arg;
    char extra_input[256];

    while (quote_count % 2 != 0)
    {
        extra_input[0] = '\0';
        ft_printf("quote> \n");
        read(0, extra_input, 256);
        extra_input[strcspn(extra_input, "\n")] = 0;
        quote_count += count_quotes(extra_input, quote_type); 

        char *temp_arg = malloc(ft_strlen(new_arg) + ft_strlen(extra_input) + 1);
        strcpy(temp_arg, new_arg);
        strcat(temp_arg, extra_input);
        free(new_arg);
        new_arg = temp_arg;
    }

    return new_arg;
}

char *replace_env_vars(char *arg, char quote_type)
{
    char *result = malloc(ft_strlen(arg) + 1);
    int in_quotes = 0;
    int i = 0, j = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == quote_type)
        {
            in_quotes = !in_quotes;
        }
        else if (arg[i] == '$' && in_quotes && quote_type == '\"')
        {
            char var_name[256];
            int k = 0;
            i++;
            while (arg[i] != ' ' && arg[i] != quote_type && arg[i] != '\0')
            {
                var_name[k++] = arg[i++];
            }
            var_name[k] = '\0';
            char *var_value = getenv(var_name);
            if (var_value != NULL)
            {
                strcpy(result + j, var_value);
                j += ft_strlen(var_value);
            }
        }
        else
        {
            result[j++] = arg[i];
        }
        i++;
    }
    result[j] = '\0';
    return result;
}

char *handle_quotes(char *arg, char quote_type)
{
    int		quote_count;
    char	*new_arg;
    char	*result;

    quote_count = count_quotes(arg, quote_type);
    new_arg = handle_odd_quotes(arg, quote_count, quote_type);
    result = replace_env_vars(new_arg, quote_type);
    if (new_arg != arg)
        free(new_arg);
    return (result);
}

struct s_cmd	*parseexec(char **ps, char *es)
{
	char				*q;
	char				*eq;
	char				*arg;
	char				*processed_arg;
	int					tok;
	int					argc;
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;
	char				*ps_clone;
	char				*es_clone;

	ps_clone = *ps;
	es_clone = es;
	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	ret = parseredirs(ret, ps, es);
	argc = ft_count_argc(&ps_clone, es_clone);
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	argc = 0;
	while (!peek(ps, es, "|"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			write(2, "syntax error\n", 12);
			exit(-1);
		}
		arg = mkcopy(q, eq);
		processed_arg = handle_quotes(arg, '\''); // обработка одинарных кавычек
        processed_arg = handle_quotes(processed_arg, '\"'); // обработка двойных кавычек
		cmd->argv[argc] = processed_arg;
		argc++;
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	return (ret);
}
