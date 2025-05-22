/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:33:19 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/23 08:48:31 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *) b;
	while (len > 0)
	{
		*(ptr++) = (unsigned char) c;
		len--;
	}
	return (b);
}

/*int	main()
{
	char	str[50] = "Hello World!";
	printf("Before memset: %s\n", str);

	ft_memset(str, '-', 5);
	printf("After memset: %s\n", str);
	return (0);
}*/