/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/24 20:35:27 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*replace_env_vars(char *arg, char q_ty, int in_q, char **envp)
{
	char	*var_val;
	char	*result;
	int		i[2];
	int		ch_va[4];

	result = malloc((calculate_buffer_size(arg, q_ty, 0, envp)) + 1);
	if (!result)
		return (NULL);
	replace_env_vars_set(&i[0], &i[1], &ch_va[2], &ch_va[3]);
	while (arg[i[0]] != '\0')
	{
		if (calculate_buf_if(&i[0], &ch_va[2], &ch_va[3], q_ty, arg))
			in_q = !in_q;
		else if (arg[i[0]] == '$' && ((!in_q && q_ty == '\'') || (in_q
					&& q_ty == '\"')))
		{
			if (replace_env_vars_exit_code(&ch_va[0], arg, &i[0], &var_val))
				var_val = handle_env_var(arg, &i[0], &ch_va[1], envp);
			if (re_e_v_ch_em(var_val, &result, &i[1]) && (ch_va[0] || ch_va[1]))
				free(var_val);
		}
		else
			result[i[1]++] = arg[i[0]++];
	}
	return (replace_env_vars_return(&result, i[1]));
}

char	*handle_quotes(char *arg, char quote_type, char **envp)
{
	int		quote_count;
	char	*new_arg;
	char	*result;

	quote_count = count_quotes(arg, quote_type);
	new_arg = handle_odd_quotes(arg, quote_count, quote_type);
	result = replace_env_vars(new_arg, quote_type, 0, envp);
	if (new_arg != arg)
		free(new_arg);
	return (result);
}

int	parseexec_count_argc(char **ps, char *es)
{
	char	*ps_clone;
	char	*es_clone;

	ps_clone = *ps;
	es_clone = es;
	return (ft_count_argc(&ps_clone, es_clone));
}

int	parseexec_tok(char **q, char **eq, char **ps, char *es)
{
	int	tok;

	tok = gettoken(ps, es, q, eq);
	if (tok == 0)
		return (1);
	if (tok != 'a')
		handle_error("syntax\n");
	return (0);
}

char	*parseexec_arg_process(char **q, char **eq, char ***envp)
{
	char	*arg;
	char	*processed_arg;

	arg = mkcopy((*q), (*eq));
	processed_arg = handle_all_quotes(arg, (*envp));
	free(arg);
	return (processed_arg);
}
