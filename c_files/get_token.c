/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:14 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/24 00:41:48 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void	skip_non_special_tokens(char **s, char *es)
{
	while (*s < es && !is_whitespace(**s) && !ft_strchr("<|>;", **s))
	{
		if (**s == '\"')
		{
			(*s)++;
			while (*s < es && **s != '\"')
				(*s)++;
		}
		else if (**s == '\'')
		{
			(*s)++;
			while (*s < es && **s != '\'')
				(*s)++;
		}
		if (*s < es)
			(*s)++;
	}
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	int		token;
	char	*s;

	s = *ps;
	while (s < es && is_whitespace(*s))
		s++;
	if (q)
		*q = s;
	token = *s;
	process_special_tokens(&s, &token);
	if (token == 'a')
		skip_non_special_tokens(&s, es);
	if (eq)
		*eq = s;
	while (s < es && is_whitespace(*s))
		s++;
	*ps = s;
	return (token);
}

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
	if (command->envp)
		free_envp(command->envp);
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
