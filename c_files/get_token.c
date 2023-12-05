/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:14 by idlbltv           #+#    #+#             */
/*   Updated: 2023/12/01 19:58:40 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

void	process_special_tokens(char **s, int *token)
{
	if (*token == '\0')
		*token = '\0';
	else if (*token == '|')
		(*s)++;
	else if (*token == ';')
		(*s)++;
	else if (*token == '>')
	{
		(*s)++;
		if (**s == '>')
		{
			*token = '+';
			(*s)++;
		}
	}
	else if (*token == '<')
	{
		(*s)++;
		if (**s == '<')
		{
			*token = '%';
			(*s)++;
		}
	}
	else
		*token = 'a';
}

void	skip_non_special_tokens(char **s, char *es)
{
	while (*s < es && !is_whitespace(**s) && !ft_strchr("<|>", **s))
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

void	free_cmd(struct s_cmd *command)
{
	struct s_pipecmd	*pcmd;
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
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
	else if (command->type == '|')
	{
		pcmd = (struct s_pipecmd *)command;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (command->type == '>' || command->type == '<' || command->type == '+' || command->type == '%')
	{
		rcmd = (struct s_redircmd *)command;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
	}
	if (command->envp)
		free_envp(command->envp);
	free(command);
}

void free_envp(char **envp)
{
	int i;

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
