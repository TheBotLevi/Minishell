/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:52:26 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 15:18:36 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!haystack && !len)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		while (haystack[i + j] == needle[j] && haystack[i + j] && i + j < len)
		{
			j++;
			if (needle[j] == 0)
				return ((char *) haystack + i);
		}
		i++;
		j = 0;
	}
	return (NULL);
}

/*int	main()
{
	char	*hs = "Hello World!";
	char	*nd = "Hello";

	char	*result = ft_strnstr(hs, nd, 20);

	if (result != NULL)
		printf("Found '%s' in main String %s", nd, result);
	else
		printf("Could not find '%s' in the String.", nd);
	return (0);
}*/