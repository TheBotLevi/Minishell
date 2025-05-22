/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:00:41 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/30 09:09:17 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str1;
	unsigned char	str2;
	size_t			i;

	str1 = (unsigned char *) s;
	str2 = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (str1[i] == str2)
			return ((void *)&str1[i]);
		i++;
	}
	return (0);
}

/*int	main()
{
	const void	*str = "Goat Messi!";
	int	i = 'o';

	char	*result = (char *)ft_memchr(str, i, 5);

	if (result != NULL)
		printf("Found '%c': %s\n", i, result);
	else
		printf("Character '%c' not found in the first 5 bytes.\n", i);
	return (0);
}*/