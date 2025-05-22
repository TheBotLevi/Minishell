/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 08:13:26 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/30 09:38:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write (fd, &s[i], 1);
		s++;
	}
	write (fd, "\n", 1);
}

/*int	main()
{
	ft_putendl_fd("Hello", 1);
	ft_putendl_fd("John", 1);
	ft_putendl_fd("Porc", 1);
	return (0);
}*/