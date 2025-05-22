/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:23:09 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/25 15:01:56 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*dest;

	len = ft_strlen(s1);
	i = 0;
	dest = (char *)malloc(len + 1);
	if (!dest)
		return (0);
	while (i < len)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*int	main()
{
	const char	*original = "Hello, World!";
	char	*copy;

	copy = ft_strdup(original);

	if (!copy)
	{
		printf("Memory allocation failed!\n");
		return (1);
	}
	printf("Original: %s\n", original);
	printf("Copy: %s\n", copy);
	free(copy);
	return (0);
}*/