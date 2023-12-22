/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:30:35 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/22 17:16:12 by azhadan          ###   ########.fr       */
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
			return (argc);
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
	g_exit_code = (argc == 2) ? ft_atoi(args[1]) : g_exit_code;
	if (g_exit_code < 0)
	{
		g_exit_code = (256 + g_exit_code) % 256;
	}
	else
	{
		g_exit_code %= 256;
	}
	ft_free_strs(args);
	return (0);
}

void	clean_builtin(char ***vars, char **processed_var)
{
	ft_free_strs((*vars));
	free((*processed_var));
}

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
	char	*temp;
	int		res;

	trimmed_buf = trim_spaces(buf);
	processed_var = handle_quotes(trimmed_buf, '\'', *envp);
	temp = processed_var;
	processed_var = handle_quotes(processed_var, '\"', *envp);
	free(temp);
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

char	*builtin_getenv(const char *var, char **envp)
{
	int	i;

	if (var == NULL)
	{
		return (NULL);
	}
	for (i = 0; envp[i]; i++)
	{
		if (!strncmp(envp[i], var, strlen(var)) && envp[i][strlen(var)] == '=')
		{
			return (&envp[i][strlen(var) + 1]);
		}
	}
	return (NULL);
}
