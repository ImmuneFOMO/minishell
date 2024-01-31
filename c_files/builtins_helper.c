/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:29:02 by azhadan           #+#    #+#             */
/*   Updated: 2024/01/30 22:28:50 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	ft_count_argc(char **ps, char *es)
{
	char	*q;
	char	*eq;
	int		tok;
	int		argc;

	argc = 0;
	while (!peek(ps, es, "|") && !peek(ps, es, ">") && !peek(ps, es, "<")
		&& !peek(ps, es, ";") && !peek(ps, es, ">>") && !peek(ps, es, "<<"))
	{
		tok = gettoken(ps, es, &q, &eq);
		if (tok == 0)
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

int	is_valid_number(char *str)
{
	if (!str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	builtin_exit_errors(int argc, char ***args)
{
	if (argc > 2)
		ft_putendl_fd("exit: too many arguments", 2);
	else
		ft_putendl_fd("exit: numeric argument required", 2);
	ft_free_strs((*args));
	g_exit_code = 1;
	if (argc <= 2)
		g_exit_code = 2;
}

int	builtin_exit(char *buf)
{
	char	**args;
	int		argc;

	args = ft_split(buf, ' ');
	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2 || (argc == 2 && !is_valid_number(args[1])))
	{
		builtin_exit_errors(argc, &args);
		return (1);
	}
	if (argc == 2)
		g_exit_code = ft_atoi(args[1]);
	if (g_exit_code < 0)
		g_exit_code = (256 + g_exit_code) % 256;
	else
		g_exit_code %= 256;
	ft_free_strs(args);
	return (0);
}

void	clean_builtin(char ***vars, char **processed_var)
{
	ft_free_strs((*vars));
	free((*processed_var));
}
