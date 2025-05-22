/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prt_ptr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:12:46 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:19:22 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	p_len(unsigned long long n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 16;
		digits += 1;
	}
	return (digits);
}

static void	put_ptr(unsigned long long addr)
{
	static char	digits[] = "0123456789abcdef";

	if (addr > 15)
		put_ptr(addr / 16);
	write (1, &digits[addr % 16], 1);
}

int	prt_ptr(void *pt)
{
	if (pt == NULL)
		return (write (1, "(nil)", 5));
	write (1, "0x", 2);
	put_ptr((unsigned long long)pt);
	return (p_len((unsigned long long)pt) + 2);
}
