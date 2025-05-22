/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:54:54 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/23 08:55:00 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = ft_strlen(src);
	if (dstsize > i + 1)
		ft_memcpy((void *) dst, (const void *) src, i + 1);
	else if (dstsize != 0)
	{
		ft_memcpy((void *)dst, (const void *)src, dstsize - 1);
		dst[dstsize - 1] = 0;
	}
	return (i);
}

/*int	main()
{
	char	dst[50] = "Hello,";
	char	src[] = "world!";

	ft_strlcpy(dst, src, 7);
	dst[] = '\0';

	printf("%s", dst);
	return (0);
}*/