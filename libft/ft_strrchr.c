/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:49:53 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/25 16:38:30 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len;
	char	i;

	len = (int)ft_strlen(s);
	i = (char ) c;
	while (len >= 0)
	{
		if (s[len] == i)
		{
			return ((char *)&s[len]);
		}
		len--;
	}
	return (NULL);
}

/*int	main()
{
	const char	str[] = "Hello everyone!";
	char	*result = ft_strrchr(str, 'o');

	if (result)
		printf("Last occurence of 'o': %s\n", result);
	else
		printf("'o' not found!\n");
	
	result = ft_strrchr(str, 'z');

	if (result)
		printf("Last occurence of 'z': %s\n", result);
	else
		printf("'z' not found!\n");
	return (0);
}*/
