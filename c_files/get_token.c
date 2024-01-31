/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelibal <idelibal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 23:30:14 by idlbltv           #+#    #+#             */
/*   Updated: 2024/01/31 21:44:39 by idelibal         ###   ########.fr       */
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

void	process_special_tokens(char **s, int *token)
{
	if (*token == '|')
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
	else if (*token != '\0')
		*token = 'a';
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

int	check_vars(char **vars)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		if (!ft_strcmp(vars[i], "|") || !ft_strcmp(vars[i], ">")
			|| !ft_strcmp(vars[i], "<") || !ft_strcmp(vars[i], "<<")
			|| !ft_strcmp(vars[i], ">>"))
			return (1);
		i++;
	}
	return (0);
}
