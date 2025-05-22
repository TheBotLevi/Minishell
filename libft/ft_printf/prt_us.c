/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prt_us.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:58:38 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:19:32 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	u_len(unsigned int n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		digits += 1;
	}
	return (digits);
}

void	put_us(unsigned int nbr)
{
	static char	digits[] = "0123456789";

	if (nbr > 9)
		put_us(nbr / 10);
	write(1, &digits[nbr % 10], 1);
}

int	prt_us(unsigned int nbr)
{
	put_us(nbr);
	return (u_len(nbr));
}
