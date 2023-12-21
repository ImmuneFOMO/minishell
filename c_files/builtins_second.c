/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_second.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:37:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/21 22:34:05 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int is_valid_var_name(char *var_name)
{
    if (!var_name || !((var_name[0] >= 'a' && var_name[0] <= 'z') || (var_name[0] >= 'A' && var_name[0] <= 'Z'))) {
        return 0;
    }

    for (int i = 1; var_name[i]; i++) {
        if (!((var_name[i] >= '0' && var_name[i] <= '9') || (var_name[i] >= 'a' && var_name[i] <= 'z') || (var_name[i] >= 'A' && var_name[i] <= 'Z') || var_name[i] == '_')) {
            return 0;
        }
    }
    return 1;
}

int builtin_export_name_valid(int var_name_length, char **var, char ***var_val)
{
    char var_name[var_name_length + 1];
    
    ft_strncpy(var_name, (*var), var_name_length);
    var_name[var_name_length] = '\0';
    if (!is_valid_var_name(var_name)) 
    {
        g_exit_code = 1;
        ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
        return (1);
    }
    (*var_val) = ft_split((*var), '=');
    if ((*var_val) == NULL || (*var_val)[0] == NULL)
    {
        ft_free_strs((*var_val));
        return (2);
    }
    return (0);
}

int check_for_equal_sign(char *var)
{
    char *eq_pos = ft_strchr(var, '=');
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

int update_or_add_env_var(char ***envp, char ***var_val)
{
    int j = 0;

    while ((*envp)[j]) {
        char **env_var_val = ft_split((*envp)[j], '=');
        if (!env_var_val) {
            ft_free_strs((*var_val));
            return (0);
        }
        if (env_var_val[0] && ft_strcmp(env_var_val[0], (*var_val)[0]) == 0) {
            return update_environment_variable(envp, var_val, j, &env_var_val);
        }
        ft_free_strs(env_var_val);
        j++;
    }

    return add_new_environment_variable(envp, var_val, j);
}

int update_environment_variable(char ***envp, char ***var_val, int index, char ***env_var_val)
{
    char *temp; 
    char *new_val;
    
    free((*envp)[index]);
    temp = ft_strjoin((*var_val)[0], "=");
    new_val = (*var_val)[1] ? ft_strjoin(temp, (*var_val)[1]) : ft_strjoin(temp, "");
    free(temp);
    if (!new_val) 
    {
        ft_free_strs((*env_var_val));
        ft_free_strs((*var_val));
        return (0);
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

int add_new_environment_variable(char ***envp, char ***var_val, int size)
{
    char *temp;
    char *new_val;
    char **new_envp = malloc((size + 2) * sizeof(char *));
    if (!new_envp) 
    {
        ft_free_strs((*var_val));
        return (0);
    }
    for (int i = 0; i < size; i++) {
        new_envp[i] = ft_strdup((*envp)[i]);
    }
    temp = ft_strjoin((*var_val)[0], "=");
    new_val = (*var_val)[1] ? ft_strjoin(temp, (*var_val)[1]) : ft_strjoin(temp, "");
    free(temp);

    if (!new_val) {
        free(new_envp);
        ft_free_strs((*var_val));
        return (1);
    }

    new_envp[size] = ft_strdup(new_val);
    free(new_val);
    if (!new_envp[size]) {
        free(new_envp);
        ft_free_strs((*var_val));
        return (1);
    }

    new_envp[size + 1] = NULL;
    ft_free_strs((*envp));
    (*envp) = new_envp;
    ft_free_strs((*var_val));
    return (0);
}

int builtin_export(char *var, char ***envp)
{
    char    **var_val;
    int     var_name_length;
    char    *eq_pos;

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
    int j = builtin_export_name_valid(var_name_length, &var, &var_val);
    if (j != 0)
        return j;

    return update_or_add_env_var(envp, &var_val);
}

void ft_free_strs(char **strs)
{
    int i;
    
    i = 0;
	while (strs[i])
	{
	free(strs[i]);
	i++;
	}
	free(strs);
}
