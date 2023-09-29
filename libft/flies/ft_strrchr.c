/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 17:04:33 by azhadan           #+#    #+#             */
/*   Updated: 2023/06/15 02:40:43 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}
// int	main(void)
// {
// 	const char	*str;
// 	str = "Hello, world!";
// 	char *ptr1, *ptr2;
// 	ptr1 = strrchr(str, 'a');
// 	ptr2 = ft_strrchr(str, 'a');
// 	printf("origin:%p\n", ptr1);
// 	printf("    my:%p\n", ptr2);
// }
