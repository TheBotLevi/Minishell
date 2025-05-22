/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:52:12 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/23 08:50:41 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	if (dst == (void *)0 && src == (void *)0)
		return (dst);
	ptr1 = (unsigned char *) dst;
	ptr2 = (unsigned char *) src;
	while (n > 0)
	{
		*(ptr1++) = *(ptr2++);
		n--;
	}
	return (dst);
}

/*int	main()
{
	char	dst[50] = "Hello";
	char	src[] = "world!";

	ft_memcpy(dst, src, 5);
	dst[5] = '\0';

	printf("%s", dst);
	return (0);
}*/