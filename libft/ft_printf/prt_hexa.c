/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prt_hexa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 07:58:19 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:19:09 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	h_len(unsigned int n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		digits += 1;
		n = n / 16;
	}
	return (digits);
}

static void	put_hexa(unsigned int nb, bool upper_case)
{
	static char	upper_digits[] = "0123456789ABCDEF";
	static char	lower_digits[] = "0123456789abcdef";

	if (nb >= 16)
		put_hexa((nb / 16), upper_case);
	if (upper_case == true)
		write (1, &upper_digits[nb % 16], 1);
	else
		write (1, &lower_digits[nb % 16], 1);
}

int	prt_hexa(unsigned int nb, bool upper_case)
{
	put_hexa(nb, upper_case);
	return (h_len(nb));
}
