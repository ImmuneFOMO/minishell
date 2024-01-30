/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 03:03:12 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/23 23:43:04 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_count(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*ft_get_word(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (ft_substr(s, 0, len));
}

char	**ft_split(const char *s, char c)
{
	int		word_count;
	char	**result;
	int		i;

	word_count = ft_word_count(s, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	if (!s || !result)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			result[i++] = ft_get_word(s, c);
			if (!result[i - 1])
				return (NULL);
			while (*s && *s != c)
				s++;
		}
	}
	result[i] = NULL;
	return (result);
}
