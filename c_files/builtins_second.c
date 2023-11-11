/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_second.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:37:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/11/11 01:44:35 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void builtin_export(char *var, char ***envp)
{
    char **var_val;
    int j;

    var_val = ft_split(var, '=');
    if (!var_val || !var_val[0] || !var_val[1])
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
        if (env_var_val[0] && strcmp(env_var_val[0], var_val[0]) == 0)
        {
            free((*envp)[j]);
            char *temp = ft_strjoin(var_val[0], "=");
            char *new_val = ft_strjoin(temp, var_val[1]);
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

    char **new_envp = malloc((j + 1) * sizeof(char *));
    if (!new_envp)
    {
        ft_free_strs(var_val);
        return;
    }
    for (int i = 0; i < j; i++)
    {
        new_envp[i] = ft_strdup((*envp)[i]);
    }
    new_envp[j + 1] = NULL; // Setting the last element to NULL

    // Creating the new variable
    char *temp = ft_strjoin(var_val[0], "=");
    char *new_val = ft_strjoin(temp, var_val[1]);
    free(temp);

    if (!new_val)
    {
        free(new_envp);
        ft_free_strs(var_val);
        return;
    }

    new_envp[j] = strdup(new_val);
    free(new_val);

    if (!new_envp[j])
    {
        free(new_envp);
        ft_free_strs(var_val);
        return;
    }

    j = 0;
    while ((*envp)[j])
        free((*envp)[j++]);
    free(*envp);
    *envp = new_envp;
    ft_free_strs(var_val);
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
