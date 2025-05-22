/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 08:53:34 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/25 15:49:34 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*ptr1;
	char	*ptr2;
	size_t	i;

	if (!src && !dst)
		return (0);
	ptr1 = (char *) dst;
	ptr2 = (char *) src;
	i = 0;
	if (ptr1 > ptr2)
	{
		while (len-- > 0)
			ptr1[len] = ptr2[len];
	}
	else
	{
		while (i < len)
		{
			ptr1[i] = ptr2[i];
			i++;
		}
	}
	return (dst);
}

/*int	main()
{
	char	dst[50] = "Hello,";
	char	src[] = "world!";

	ft_memmove(dst, src, 5);
	dst[5] = '\0';

	printf("%s", dst);
	return (0);
}*/