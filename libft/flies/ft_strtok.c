/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:33:08 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/28 22:50:58 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strtok(char *str,const char *sepa)
{
	static char	*saveptr;
	char		*end;

	if (!str)
		str = saveptr;
	if (!str)
		return (NULL);
	while (*str && ft_strchr(sepa, *str))
		str++;
	if (!*str)
	{
		saveptr = NULL;
		return (NULL);
	}
	end = str;
	while (*end && !ft_strchr(sepa, *end))
		end++;
	if (*end)
	{
		*end = '\0';
		saveptr = end + 1;
	}
	else
		saveptr = NULL;
	return (str);
}
