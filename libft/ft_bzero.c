/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:20:17 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/21 15:45:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*ptr;

	ptr = (char *)s;
	while (n-- > 0)
		*(ptr++) = 0;
}

/*int	main()
{
	char	str[50] = "Hello World!";
	printf("Before bzero: %s\n", str);

	ft_bzero(str, 5);
	printf("After bzero: %s\n", str);
	return (0);
}*/