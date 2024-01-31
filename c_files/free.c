/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 19:21:13 by idelibal          #+#    #+#             */
/*   Updated: 2024/01/31 19:24:35 by idelibal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	free_cmd_checker(struct s_cmd *command)
{
	struct s_redircmd		*rcmd;
	struct s_semicoloncmd	*scmd;
	struct s_pipecmd		*pcmd;

	if (command->type == '|')
	{
		pcmd = (struct s_pipecmd *)command;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (command->type == ';')
	{
		scmd = (struct s_semicoloncmd *)command;
		free_cmd(scmd->left);
		free_cmd(scmd->right);
	}
	else if (command->type == '>' || command->type == '<'
		|| command->type == '+' || command->type == '%')
	{
		rcmd = (struct s_redircmd *)command;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
	}
}

void	free_cmd(struct s_cmd *command)
{
	struct s_execcmd	*ecmd;
	int					i;

	i = 0;
	if (!command)
		return ;
	if (command->type == ' ')
	{
		ecmd = (struct s_execcmd *)command;
		while (ecmd->argv[i])
		{
			free(ecmd->argv[i]);
			i++;
		}
		free(ecmd->argv);
	}
	free_cmd_checker(command);
	free(command);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (envp)
	{
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
		free(envp);
	}
}
