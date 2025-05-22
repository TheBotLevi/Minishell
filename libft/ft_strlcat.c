/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:34:16 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 15:23:53 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d_len;
	size_t	s_len;

	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	if (d_len >= dstsize)
		return (dstsize + s_len);
	if (s_len < dstsize - d_len)
		ft_memcpy(dst + d_len, src, s_len + 1);
	else
	{
		ft_memcpy(dst + d_len, src, dstsize - d_len - 1);
		dst[dstsize - 1] = '\0';
	}
	return (d_len + s_len);
}

/*int	main()
{
	char	dst[50] = "Hello,";
	char	src[] = "world!";
	printf("Before: %s\n", dst);

	ft_strlcat(dst, src, 20);
	printf("After: %s\n", dst);
	return (0);
}*/