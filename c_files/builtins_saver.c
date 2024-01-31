/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_saver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:41:58 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/31 18:12:43 by idelibal         ###   ########.fr       */
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
		if (!ft_strncmp(envp[i], var, ft_strlen(var)) \
			&& envp[i][ft_strlen(var)] == '=')
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

void	free_cmd_argv(struct s_execcmd *cmd, int argc)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
	free(cmd);
}

struct s_cmd *initialize_cmd(char **ps, char *es, struct s_execcmd **cmd)
{
	struct s_cmd *ret;

	ret = execcmd();
	*cmd = (struct s_execcmd *)ret;
	ret = parseredirs(ret, ps, es);
	if (ret == NULL)
	{
		free(*cmd);
		return (NULL);
	}
	return ret;
}

void allocate_argv(struct s_execcmd *cmd, char **ps, char *es)
{
	int argc;

	argc = parseexec_count_argc(ps, es);
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
}

struct s_cmd *finalize_cmd(struct s_execcmd *cmd, struct s_cmd *ret, int argc)
{
	cmd->argv[argc] = 0;
	if (ret == NULL)
	{
		free_cmd_argv(cmd, argc);
		return (NULL);
	}
	return (ret);
}

struct s_cmd	*parseexec(char **ps, char *es, char **envp)
{
	char				*q;
	char				*eq;
	int					argc;
	struct s_execcmd	*cmd;
	struct s_cmd		*ret;

	ret = initialize_cmd(ps, es, &cmd);
	if (ret == NULL)
		return NULL;
	allocate_argv(cmd, ps, es);
	argc = 0;
	while (!peek(ps, es, "|") && !peek(ps, es, ";") && !peek(ps, es, "<>"))
	{
		if (parseexec_tok(&q, &eq, ps, es))
			break ;
		cmd->argv[argc] = parseexec_arg_process(&q, &eq, &envp);
		argc++;
		ret = parseredirs(ret, ps, es);
	}
	return finalize_cmd(cmd, ret, argc);
}
