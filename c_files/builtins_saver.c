/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_saver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:41:58 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/24 00:10:55 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	*builtin_getenv(const char *var, char **envp)
{
	int	i;

	if (var == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		if (!strncmp(envp[i], var, strlen(var)) && envp[i][strlen(var)] == '=')
		{
			return (&envp[i][strlen(var) + 1]);
		}
		i++;
	}
	return (NULL);
}

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

struct s_cmd	*parseexec(char **ps, char *es, char **envp)
{
	char				*q;
	char				*eq;
	int					argc;
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;

	ret = execcmd();
	cmd = (struct s_execcmd *)ret;
	ret = parseredirs(ret, ps, es);
	argc = parseexec_count_argc(ps, es);
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	argc = 0;
	while (!peek(ps, es, "|") && !peek(ps, es, ";"))
	{
		if (parseexec_tok(&q, &eq, ps, es))
			break ;
		cmd->argv[argc] = parseexec_arg_process(&q, &eq, &envp);
		argc++;
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	return (ret);
}
