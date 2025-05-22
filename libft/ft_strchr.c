/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 09:07:31 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/18 13:28:23 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	j;
	int		i;

	i = 0;
	j = (char) c;
	while (s[i])
	{
		if (j == s[i])
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == j)
		return ((char *)&s[i]);
	return (0);
}

/*int	main()
{
	const char	*str = "Hello, World!";
	int	i = 'o';

	char	*result = ft_strchr(str, i);

	if (result != NULL)
		printf("Found '%c' in the String: %s\n", i, result);
	else
		printf("Character '%c' not found in String: %s\n", i, str);
	return (0);
}*/