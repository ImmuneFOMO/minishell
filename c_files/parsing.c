/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/17 14:47:09 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	handle_error(const char *error_message)
{
	write(2, error_message, ft_strlen(error_message));
	exit(2);
}

struct s_cmd	*parsecmd(char *s)
{
	char			*es;
	struct s_cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parseline(&s, es);
	peek(&s, es, "");
	if (s != es)
		handle_error("leftovers: \n");
	return (cmd);
}

struct s_cmd	*parseline(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parsesemicolon(ps, es);
	return (cmd);
}

struct s_cmd *parsesemicolon(char **ps, char *es)
{
    struct s_cmd    *cmd;
    int             tok;

    cmd = parsepipe(ps, es);
    if (peek(ps, es, ";"))
    {
        tok = gettoken(ps, es, 0, 0);
        if (tok == ';' && peek(ps, es, ";"))
            handle_error("syntax error near unexpected token `;;'\n");
        cmd = semicoloncmd(cmd, parsesemicolon(ps, es));
    }
    return cmd;
}


struct s_cmd	*parsepipe(char **ps, char *es)
{
	struct s_cmd	*cmd;

	cmd = parseexec(ps, es);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		if (peek(ps, es, "|") || *ps == es)
				handle_error("syntax error near unexpected token `|'\n");
		cmd = pipecmd(cmd, parsepipe(ps, es));
	}
	return (cmd);
}

struct	s_cmd *parseredirs(struct s_cmd *cmd, char **ps, char *es)
{
	int     tok;
	int     next_tok;
	char    *q;
	char    *eq;
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (peek(ps, es, ">")) {
			next_tok = gettoken(ps, es, 0, 0);
			if (next_tok == '>')
				handle_error("syntax error near unexpected token `>'\n");
			else 
				handle_error("syntax error near unexpected token `>>'\n");
		}
		if (gettoken(ps, es, &q, &eq) != 'a')
			handle_error("syntax error near unexpected token `newline'\n");
		if (tok == '<' || tok == '>' || tok == '+' || tok == '%')
			cmd = redircmd(cmd, mkcopy(q, eq), tok);
	}
	return (cmd);
}

int	count_quotes(char *arg, char quote_type)
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

char	*handle_odd_quotes(char *arg, int quote_count, char quote_type)
{
	char *new_arg;
	new_arg = arg;
	char extra_input[256];
	char *temp_arg;
	ssize_t len;

	while (quote_count % 2 != 0)
	{
		extra_input[0] = '\0';
		ft_printf("quote> ");
		len = read(0, extra_input, 255);
		if (extra_input[len - 1] == '\n') 
			extra_input[len - 1] = '\0';
		quote_count += count_quotes(extra_input, quote_type); 
		temp_arg = (char *)malloc(ft_strlen(new_arg) + ft_strlen(extra_input) + 2);
		ft_strcpy(temp_arg, new_arg);
		ft_strcat(temp_arg, "\n");
		ft_strcat(temp_arg, extra_input);

		if (new_arg != arg)
			free(new_arg);
		new_arg = temp_arg;
	}
	return (new_arg);
}

char *handle_env_var(char *arg, int *i, int *memory_allocated)
{
    char var_name[256];
    int k = 0;
    char *env_value;
    (*i)++;
    while (ft_isalnum(arg[*i]) || arg[*i] == '_')
        var_name[k++] = arg[(*i)++];
    var_name[k] = '\0';
	if (var_name[0] == '\0')
	{
		*memory_allocated = 1;
		return (ft_strdup("$"));
	}
    env_value = getenv(var_name);
    if (env_value != NULL) {
        *memory_allocated = 1;
        return ft_strdup(env_value);
    } else {
        *memory_allocated = 1;
        return NULL;
    }
}

int	calculate_buffer_size(char *arg, char quote_type, int in_quotes)
{
    char	*var_value;
    int		i;
    int		size;
    int     memory_allocated;

    i = 0;
    size = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == quote_type)
        {
            in_quotes = !in_quotes;
            i++;
            continue ;
        }
        else if (arg[i] == '$' && ((!in_quotes && quote_type == '\'') || (in_quotes && quote_type == '\"')))
        {
            if (ft_strncmp(arg + i, "$?", 2) == 0) {
                size += 3;
                i += 2;
            } else {
                var_value = handle_env_var(arg, &i, &memory_allocated);
                if (var_value != NULL)
                {
                    size += ft_strlen(var_value);
                    if (memory_allocated) {
                        free(var_value);
                    }
                }
            }
            continue ;
        }
        size++;
        i++;
    }
    return (size + 1);
}

char *replace_env_vars(char *arg, char quote_type, int in_quotes)
{
    char	*var_value;
    char	*result;
    int		size;
    int		i;
    int		j;
    int     is_itoa;
    int     memory_allocated;

    size = calculate_buffer_size(arg, quote_type, 0);
    result = malloc(size + 1);
    if (!result) return NULL;
    i = 0;
    j = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == quote_type)
        {
            in_quotes = !in_quotes;
            i++;
            continue;
        }
        else if (arg[i] == '$' && ((!in_quotes && quote_type == '\'') || (in_quotes && quote_type == '\"')))
        {
            is_itoa = 0;
            if (ft_strncmp(arg + i, "$?", 2) == 0) {
                var_value = ft_itoa(g_exit_code);
                i += 2;
                is_itoa = 1;
            } else
                var_value = handle_env_var(arg, &i, &memory_allocated);
            if (var_value != NULL)
            {
                ft_strcpy(result + j, var_value);
                j += ft_strlen(var_value);
                if (is_itoa || memory_allocated)
                    free(var_value);
            }
            continue;
        }
        result[j++] = arg[i++];
    }
    result[j] = '\0';
    return (result);
}

char	*handle_quotes(char *arg, char quote_type)
{
	int		quote_count;
	char	*new_arg;
	char	*result;

	quote_count = count_quotes(arg, quote_type);
	new_arg = handle_odd_quotes(arg, quote_count, quote_type);
	result = replace_env_vars(new_arg, quote_type, 0);
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
	while (!peek(ps, es, "|") && !peek(ps, es, ";"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
			handle_error("syntax\n");
		arg = mkcopy(q, eq);
		processed_arg = handle_quotes(arg, '\'');
		char *temp = processed_arg;
		processed_arg = handle_quotes(processed_arg, '\"');
		cmd->argv[argc] = processed_arg;
    	free(temp);
		free(arg); 
		argc++;
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	return (ret);
}
