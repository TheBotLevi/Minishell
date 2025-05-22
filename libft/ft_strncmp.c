/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:11:34 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/25 16:50:38 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/*int	main()
{
	const char	*s1 = "Hallo";
	const char	*s2 = "Hello";

	int	result = ft_strncmp(s1, s2, 3);

	if (result == 0)
		printf("The strings are equal.\n");
	else if (result < 0)
		printf("The first string smaller.\n");
	else
		printf("The first string is greater.\n");
	return (0);
}*/