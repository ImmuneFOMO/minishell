/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:30:35 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/20 19:02:46 by azhadan          ###   ########.fr       */
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
	while (!peek(ps, es, "|") && !peek(ps, es, ">") && !peek(ps, es, "<") && !peek(ps, es, ";") && !peek(ps, es, ">>") && !peek(ps, es, "<<"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
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

int is_valid_number(char *str) 
{
    if (!str) return 0;

    if (*str == '-' || *str == '+') str++;

    while (*str) {
        if (!ft_isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int builtin_exit(char *buf) 
{
    char **args = ft_split(buf, ' ');
    int argc = 0;

    while (args[argc])
        argc++;

    if (argc > 2 || (argc == 2 && !is_valid_number(args[1]))) {
        if (argc > 2)
            ft_putendl_fd("exit: too many arguments", 2);
        else
            ft_putendl_fd("exit: numeric argument required", 2);
        ft_free_strs(args);
        g_exit_code = 1;
		if (argc <= 2)
			g_exit_code = 2;
        return 1;
    }

    g_exit_code = (argc == 2) ? ft_atoi(args[1]) : g_exit_code;
    if (g_exit_code < 0) {
        g_exit_code = (256 + g_exit_code) % 256;
    } else {
        g_exit_code %= 256;
    }
    ft_free_strs(args);
    return 0;
}

int main_builtins(char *buf, char ***envp)
{
	char	*trimmed_buf;
	char	*var;
	char	**vars;
	int		i;
	char    *processed_var;

	trimmed_buf = trim_spaces(buf);
	processed_var = handle_quotes(trimmed_buf, '\'', *envp);
    char *temp = processed_var;
    processed_var = handle_quotes(processed_var, '\"', *envp);
	free(temp);
	if (ft_cd(processed_var, *envp))
	{
		free(processed_var);
		return (1);
	}
	else if (!ft_strncmp(processed_var, "exit", 5) || !ft_strncmp(processed_var, "exit ", 5))
	{
		builtin_exit(processed_var);
		free(processed_var);
		return (2);
	}
	else if (!ft_strncmp(processed_var, "unset ", 6))
	{
		var = ft_strchr(processed_var, ' ');
		var++;
		if (var)
		{
			vars = ft_split(var, ' ');
			if (check_vars(vars))
			{
				ft_free_strs(vars);
				free(processed_var);
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
		free(processed_var);
		return (1);
	}
	else if (!ft_strncmp(processed_var, "export ", 7))
	{
		var = ft_strchr(processed_var, ' ');
		var++;
		if (var)
		{
			vars = ft_split(var, ' ');
			if (check_vars(vars))
			{
				ft_free_strs(vars);
				free(processed_var);
				return (0);
			}
			i = 0;
			while (vars[i])
			{
				if (builtin_export(vars[i] , envp))
				{
					ft_free_strs(vars);
					free(processed_var);
					return (1);
				}
				i++;
			}
			ft_free_strs(vars);
		}
		free(processed_var);
		return (1);
	}
	free(processed_var);
	return (0);
}

char **dup_envp(char **envp)
{
	int i;
	int env_count;
	char **new_envp;

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
	return(new_envp);
}

char	*find_in_path(const char *cmnd, struct s_cmd *cmd)
{
	char			*path;
	char			*temp;
	char			*dir;
	char			*full_path;
	char			*temp_path;
	struct stat		st;

	path = builtin_getenv("PATH", cmd->envp);
	if (!path)
		return (NULL);
	temp = ft_strdup(path);
	if (!temp)
		return (NULL);
	dir = ft_strtok(temp, ":");
	while (dir)
	{
		temp_path = ft_strjoin(dir, "/");
		if (!temp_path)
		{
			free(dir);
			free(temp);
			return (NULL);
		}
		full_path = ft_strjoin(temp_path, cmnd);
		free(temp_path);
		if (!full_path)
		{
			free(dir);
			free(temp);
			return (NULL);
		}
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(temp);
			return (full_path);
		}
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	free(dir);
	free(temp);
	return (NULL);
}

void builtin_unset(char *var, char ***envp) 
{
    int i;
    int j;

	i = 0;
    while ((*envp)[i]) 
	{
        if (!ft_strncmp((*envp)[i], var, ft_strlen(var)) && (*envp)[i][ft_strlen(var)] == '=') 
		{
            free((*envp)[i]);
            j = i;
            while ((*envp)[j]) 
			{
                (*envp)[j] = (*envp)[j + 1];
                j++;
            }
        } else
			i++;
    }
}

char *builtin_getenv(const char *var, char **envp)
{
    int i;

    if (var == NULL) {
        return NULL;
    }

    for (i = 0; envp[i]; i++) {
        if (!strncmp(envp[i], var, strlen(var)) && envp[i][strlen(var)] == '=') {
            return &envp[i][strlen(var) + 1];
        }
    }

    return NULL;
}
