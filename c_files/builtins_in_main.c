/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_in_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:34:44 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/24 20:11:14 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	builtin_processing_unset(char **processed_var, char ***envp)
{
	int		i;
	char	*var;
	char	**vars;

	var = ft_strchr((*processed_var), ' ');
	var++;
	if (var)
	{
		vars = ft_split(var, ' ');
		if (check_vars(vars))
		{
			clean_builtin(&vars, processed_var);
			return (0);
		}
		i = 0;
		while (vars[i])
		{
			builtin_unset(vars[i], envp);
			i++;
		}
		ft_free_strs(vars);
	}
	free((*processed_var));
	return (1);
}

int	builtin_processing_export(char **processed_var, char ***envp, char **var,
		int i)
{
	char	**vars;

	(*var)++;
	if ((*var))
	{
		vars = ft_split((*var), ' ');
		if (check_vars(vars))
		{
			clean_builtin(&vars, processed_var);
			return (0);
		}
		while (vars[i])
		{
			if (builtin_export(vars[i], envp))
			{
				clean_builtin(&vars, processed_var);
				return (1);
			}
			i++;
		}
		ft_free_strs(vars);
	}
	free((*processed_var));
	return (1);
}

int	builtin_check_unset_export(char **processed_var, char ***envp)
{
	char	*var;

	if (!ft_strncmp((*processed_var), "unset ", 6))
	{
		if (builtin_processing_unset(processed_var, envp))
			return (1);
		else
			return (0);
	}
	else if (!ft_strncmp((*processed_var), "export ", 7))
	{
		var = ft_strchr((*processed_var), ' ');
		if (builtin_processing_export(processed_var, envp, &var, 0))
			return (1);
		else
			return (0);
	}
	return (3);
}

int	main_builtins_check(char **processed_var, char ***envp)
{
	if (ft_cd((*processed_var), *envp))
	{
		free((*processed_var));
		return (1);
	}
	else if (!ft_strncmp((*processed_var), "exit", 5)
		|| !ft_strncmp((*processed_var), "exit ", 5))
	{
		builtin_exit((*processed_var));
		free((*processed_var));
		return (2);
	}
	return (builtin_check_unset_export(processed_var, envp));
}

int	main_builtins(char *buf, char ***envp)
{
	char	*trimmed_buf;
	char	*processed_var;
	int		res;

	trimmed_buf = trim_spaces(buf);
	processed_var = handle_all_quotes(trimmed_buf, (*envp));
	res = main_builtins_check(&processed_var, envp);
	if (res == 0)
		return (0);
	else if (res == 2)
		return (2);
	else if (res == 1)
		return (1);
	free(processed_var);
	return (0);
}
