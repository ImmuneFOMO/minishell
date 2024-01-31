/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/31 21:03:24 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*quotes_env_errors(char *arg, char q_ty, int in_q, char **envp)
{
	char	*var_val;
	char	*result;
	int		i[2];
	int		ch_va[4];

	result = malloc((calculate_buffer_size(arg, q_ty, 0, envp)) + 1);
	if (!result)
		return (NULL);
	quotes_env_errors_set(&i[0], &i[1], &ch_va[2], &ch_va[3]);
	while (arg[i[0]] != '\0')
	{
		if (calculate_buf_if(&ch_va[2], &ch_va[3], q_ty, arg[i[0]]))
		{
			in_q = !in_q;
			i[0] = i[0] + 1;
		}
		else if (arg[i[0]] == '$' && ((!in_q && q_ty == '\'') || (in_q
					&& q_ty == '\"')))
		{
			if (quotes_env_errors_exit_code(&ch_va[0], arg, &i[0], &var_val))
				var_val = handle_env_var(arg, &i[0], &ch_va[1], envp);
			if (re_e_v_ch_em(var_val, &result, &i[1]) && (ch_va[0] || ch_va[1]))
				free(var_val);
		}
		else
			result[i[1]++] = arg[i[0]++];
	}
	return (quotes_env_errors_return(&result, i[1]));
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
