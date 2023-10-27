/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:33:08 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/27 11:39:03 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strtok(char *str, char sepa)
{
	static char	*stock = NULL;
	char		*ptr;
	int		i;

	i = 0;
	ptr = NULL;
	if (str != NULL)
		stock = ft_strdup(str);
	while (*stock != '\0')
	{
		if (i == 0 && *stock != sepa)
		{
			i = 1;
			ptr = stock;
		}
		else if (i == 1 && *stock == sepa)
		{
			*stock = '\0';
			stock += 1;
			break ;
		}
		stock++;
	}
	return (ptr);
}
