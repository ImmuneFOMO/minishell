/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_second.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:37:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/12/19 15:38:00 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

extern int g_exit_code;

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

void builtin_export(char *var, char ***envp)
{
     char    **var_val;
    char    *new_val;
    char    *temp;
    int     j;

    char *eq_pos = ft_strchr(var, '=');
    if (eq_pos == NULL)
    {
        if (!is_valid_var_name(var)) {
        g_exit_code = 1;
        ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
        exit (1);
    }
    return;
    }
    int var_name_length = eq_pos - var;
    char var_name[var_name_length + 1];
    ft_strncpy(var_name, var, var_name_length);
    var_name[var_name_length] = '\0';
    if (!is_valid_var_name(var_name)) {
        g_exit_code = 1;
        ft_putstr_fd(" not a valid identifier\n", STDERR_FILENO);
        exit (1);
    }
    var_val = ft_split(var, '=');
    if (var_val == NULL || var_val[0] == NULL)
    {
        ft_free_strs(var_val);
        return;
    }
    j = 0;
    while ((*envp)[j])
    {
        char **env_var_val = ft_split((*envp)[j], '=');
        if (!env_var_val)
        {
            ft_free_strs(var_val);
            return;
        }
        if (env_var_val[0] && ft_strcmp(env_var_val[0], var_val[0]) == 0)
        {
            free((*envp)[j]);
            temp = ft_strjoin(var_val[0], "=");
            if (var_val[1])
                new_val = ft_strjoin(temp, var_val[1]);
            else
                new_val = ft_strjoin(temp, "");
            free(temp);
            if (!new_val)
            {
                ft_free_strs(env_var_val);
                ft_free_strs(var_val);
                return;
            }
            (*envp)[j] = ft_strdup(new_val);
            free(new_val);
            if (!(*envp)[j])
            {
                ft_free_strs(env_var_val);
                ft_free_strs(var_val);
                return;
            }
            ft_free_strs(env_var_val);
            ft_free_strs(var_val);
            return;
        }
        ft_free_strs(env_var_val);
        j++;
    }

    char **new_envp = malloc((j + 2) * sizeof(char *));
    if (!new_envp)
    {
        ft_free_strs(var_val);
        return;
    }
    j = 0;
    while ((*envp)[j] != NULL)
    {
        new_envp[j] = ft_strdup((*envp)[j]);
        j++;
    }
    temp = ft_strjoin(var_val[0], "=");
    if (var_val[1])
                new_val = ft_strjoin(temp, var_val[1]);
            else
                new_val = ft_strjoin(temp, "");
    free(temp);

    if (!new_val)
    {
        free(new_envp);
        ft_free_strs(var_val);
        return;
    }

    new_envp[j] = ft_strdup(new_val);
    free(new_val);
    if (!new_envp[j])
    {
        free(new_envp);
        ft_free_strs(var_val);
        return;
    }
     new_envp[j + 1] = NULL;
    j = 0;
    ft_free_strs(*envp);
    *envp = new_envp;
    ft_free_strs(var_val);
     g_exit_code = 0;
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
