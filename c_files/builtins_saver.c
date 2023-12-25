/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_saver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:41:58 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/25 23:10:10 by root             ###   ########.fr       */
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
		if (!ft_strncmp(envp[i], var, ft_strlen(var)) && envp[i][ft_strlen(var)] == '=')
		{
			return (&envp[i][ft_strlen(var) + 1]);
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
