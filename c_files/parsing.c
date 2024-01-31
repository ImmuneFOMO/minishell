/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:37 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/31 21:10:58 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*quotes_env_errors(char *arg, char q_ty, int in_q, char **envp)
{
	char	*var_val;
	char	*result;
	int		i[6];

	result = malloc((calculate_buffer_size(arg, q_ty, 0, envp)) + 1);
	quotes_env_errors_set(&i[4], &i[5], &i[2], &i[3]);
	while (arg[i[4]] != '\0')
	{
		if (calculate_buf_if(&i[2], &i[3], q_ty, arg[i[4]]))
		{
			in_q = !in_q;
			i[4] = i[4] + 1;
		}
		else if (arg[i[4]] == '$' && ((!in_q && q_ty == '\'') || (in_q
					&& q_ty == '\"')))
		{
			if (quotes_env_errors_exit_code(&i[0], arg, &i[4], &var_val))
				var_val = handle_env_var(arg, &i[4], &i[1], envp);
			if (re_e_v_ch_em(var_val, &result, &i[5]) && (i[0] || i[1]))
				free(var_val);
		}
		else
			result[i[5]++] = arg[i[4]++];
	}
	return (quotes_env_errors_return(&result, i[5]));
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
