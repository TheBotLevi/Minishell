/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:38:12 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 11:40:15 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			total;

	i = 0;
	if (size != 0 && count > (size_t)-1 / size)
		return (NULL);
	total = count * size;
	ptr = malloc(total);
	if (!ptr)
		return (0);
	while (i < total)
		ptr[i++] = 0;
	return (ptr);
}

/*int	main()
{
	size_t	nbr = 5;
	int	*str = (int *)ft_calloc(nbr, sizeof(int));

	if (!str)
	{
		printf("Memory allocation failed!\n");
		return (1);
	}
	printf("Array values after calloc:\n");
	printf("%d ", str[0]);
	printf("%d ", str[1]);
	printf("%d ", str[2]);
	printf("%d ", str[3]);
	printf("%d\n", str[4]);
	free(str);
	return (0);
}*/