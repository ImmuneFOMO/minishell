/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:30:35 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/23 23:42:56 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

char	**dup_envp(char **envp)
{
	int		i;
	int		env_count;
	char	**new_envp;

	i = 0;
	env_count = 0;
	while (envp[env_count])
		env_count++;
	new_envp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!new_envp)
		return (NULL);
	while (i < env_count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[env_count] = NULL;
	return (new_envp);
}

char	*create_full_path(char *dir, char *command)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(dir, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, command);
	free(temp_path);
	return (full_path);
}

char	*search_command_in_dirs(char *temp, char *command)
{
	char		*dir;
	struct stat	st;
	char		*full_path;

	dir = ft_strtok(temp, ":");
	while (dir)
	{
		full_path = create_full_path(dir, command);
		if (!full_path)
			return (NULL);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
			return (full_path);
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_in_path(char *cmd)
{
	struct stat	st;
	char		*path;
	char		*temp;
	char		*result;

	if (cmd[0] == '/' || ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = getenv("PATH");
	if (!path)
		return (NULL);
	temp = ft_strdup(path);
	if (!temp)
		return (NULL);
	result = search_command_in_dirs(temp, cmd);
	free(temp);
	return (result);
}

void	builtin_unset(char *var, char ***envp)
{
	int	i;
	int	j;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], var, ft_strlen(var))
			&& (*envp)[i][ft_strlen(var)] == '=')
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
		}
		else
			i++;
	}
}
