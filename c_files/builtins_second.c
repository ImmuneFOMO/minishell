/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_second.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:37:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/22 21:35:14 by azhadan          ###   ########.fr       */
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

int	update_environment_variable(char ***envp, char ***var_val, int index,
		char ***env_var_val)
{
	char	*new_val;

	new_val = allocation_update_env_variable(envp, index, var_val);
	if (!new_val)
	{
		ft_free_strs((*env_var_val));
		ft_free_strs((*var_val));
		return (1);
	}
	(*envp)[index] = ft_strdup(new_val);
	free(new_val);
	if (!(*envp)[index])
	{
		ft_free_strs((*env_var_val));
		ft_free_strs((*var_val));
		return (0);
	}
	ft_free_strs((*env_var_val));
	ft_free_strs((*var_val));
	return (0);
}

int	add_new_env_var_finish(char **new_val, char ***var_val, char ***new_envp,
		int size)
{
	if (!(*new_val))
	{
		free((*new_envp));
		ft_free_strs((*var_val));
		return (1);
	}
	(*new_envp)[size] = ft_strdup((*new_val));
	free((*new_val));
	if (!(*new_envp)[size])
	{
		free((*new_envp));
		ft_free_strs((*var_val));
		return (1);
	}
	(*new_envp)[size + 1] = NULL;
	return (0);
}

char	**add_new_new_envp(int size, char ***envp, char ***var_val)
{
	char	**new_envp;
	int		i;

	new_envp = malloc((size + 2) * sizeof(char *));
	if (!new_envp)
	{
		ft_free_strs((*var_val));
		return (0);
	}
	i = 0;
	while (i < size)
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		i++;
	}
	return (new_envp);
}

int	add_new_environment_variable(char ***envp, char ***var_val, int size)
{
	char	*temp;
	char	*new_val;
	char	**new_envp;

	new_envp = add_new_new_envp(size, envp, var_val);
	temp = ft_strjoin((*var_val)[0], "=");
	if ((*var_val)[1])
		new_val = ft_strjoin(temp, (*var_val)[1]);
	else
		new_val = ft_strjoin(temp, "");
	free(temp);
	ft_free_strs((*envp));
	(*envp) = new_envp;
	ft_free_strs((*var_val));
	if (add_new_env_var_finish(&new_val, var_val, &new_envp, size))
		return (1);
	return (0);
}

int	builtin_export(char *var, char ***envp)
{
	char	**var_val;
	int		var_name_length;
	char	*eq_pos;
	int		j;

	eq_pos = ft_strchr(var, '=');
	if (eq_pos == NULL)
	{
		if (!is_valid_var_name(var))
		{
			g_exit_code = 1;
			ft_printf("%s", var);
			ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
			return (1);
		}
		return (0);
	}
	var_name_length = eq_pos - var;
	j = builtin_export_name_valid(var_name_length, &var, &var_val);
	if (j != 0)
		return (j);
	return (update_or_add_env_var(envp, &var_val));
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
