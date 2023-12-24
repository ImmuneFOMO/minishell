/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_second_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 23:39:23 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/23 23:40:17 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	is_valid_var_name(char *var_name)
{
	int	i;

	if (!var_name || !((var_name[0] >= 'a' && var_name[0] <= 'z')
			|| (var_name[0] >= 'A' && var_name[0] <= 'Z')))
	{
		return (0);
	}
	i = 1;
	while (var_name[i])
	{
		if (!((var_name[i] >= '0' && var_name[i] <= '9') || (var_name[i] >= 'a'
					&& var_name[i] <= 'z') || (var_name[i] >= 'A'
					&& var_name[i] <= 'Z') || var_name[i] == '_'))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	builtin_export_name_valid(int var_name_length, char **var, char ***var_val)
{
	char	*var_name;

	var_name = malloc(var_name_length + 1);
	ft_strncpy(var_name, (*var), var_name_length);
	var_name[var_name_length] = '\0';
	if (!is_valid_var_name(var_name))
	{
		g_exit_code = 1;
		ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
		free(var_name);
		return (1);
	}
	(*var_val) = ft_split((*var), '=');
	if ((*var_val) == NULL || (*var_val)[0] == NULL)
	{
		ft_free_strs((*var_val));
		free(var_name);
		return (2);
	}
	free(var_name);
	return (0);
}

int	check_for_equal_sign(char *var)
{
	char	*eq_pos;

	eq_pos = ft_strchr(var, '=');
	if (eq_pos == NULL)
	{
		if (!is_valid_var_name(var))
		{
			g_exit_code = 1;
			ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
			return (1);
		}
		return (0);
	}
	return (2);
}

int	update_or_add_env_var(char ***envp, char ***var_val)
{
	int		j;
	char	**env_var_val;

	j = 0;
	while ((*envp)[j])
	{
		env_var_val = ft_split((*envp)[j], '=');
		if (!env_var_val)
		{
			ft_free_strs((*var_val));
			return (0);
		}
		if (env_var_val[0] && ft_strcmp(env_var_val[0], (*var_val)[0]) == 0)
		{
			return (update_environment_variable(envp, var_val, j,
					&env_var_val));
		}
		ft_free_strs(env_var_val);
		j++;
	}
	return (add_new_environment_variable(envp, var_val, j));
}

char	*allocation_update_env_variable(char ***envp, int index,
		char ***var_val)
{
	char	*temp;
	char	*new_val;

	free((*envp)[index]);
	temp = ft_strjoin((*var_val)[0], "=");
	if ((*var_val)[1])
		new_val = ft_strjoin(temp, (*var_val)[1]);
	else
		new_val = ft_strjoin(temp, "");
	free(temp);
	return (new_val);
}
