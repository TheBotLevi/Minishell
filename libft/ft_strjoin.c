/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:54:08 by ljeribha          #+#    #+#             */
/*   Updated: 2025/01/14 14:02:28 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len;
	char	*join;
	char	*ptr;

	len = ft_strlen(s1) + ft_strlen(s2);
	join = (char *)malloc(len + 1);
	ptr = join;
	if (!s1 || !s2)
		return (0);
	if (!join)
		return (0);
	while (*s1)
		*ptr++ = *s1++;
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (join);
}

/*int	main()
{
	char	*s1 = "Hello, ";
	char	*s2 = "World!";
	char	*result = ft_strjoin(s1, s2);

	if (result)
		printf("Joined string: %s\n", result);
	free(result);
	return (0);
}*/