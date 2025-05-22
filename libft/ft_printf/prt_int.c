/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prt_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:01:24 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:34:52 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	i_len(int n)
{
	size_t	digits;

	digits = 0;
	if (n <= 0)
		digits += 1;
	while (n != 0)
	{
		digits += 1;
		n = n / 10;
	}
	return (digits);
}

static void	put_int(int n)
{
	static char	digits[] = "0123456789";

	if (n > 9)
		put_int(n / 10);
	write (1, &digits[n % 10], 1);
}

int	prt_int(int n)
{
	int	len;

	if (n == INT_MIN)
		return ((write (1, "-2147483648", 11)));
	len = i_len(n);
	if (n < 0)
	{
		write (1, "-", 1);
		n *= -1;
	}
	put_int(n);
	return (len);
}
