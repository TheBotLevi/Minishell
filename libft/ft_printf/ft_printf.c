/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:53:26 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:17:28 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	conversion(char c, va_list args)
{
	if (c == '%')
		return (write(1, "%", 1));
	else if (c == 'c')
	{
		ft_putchar(va_arg(args, int));
		return (1);
	}
	else if (c == 'u')
		return (prt_us(va_arg(args, unsigned int)));
	else if (c == 'p')
		return (prt_ptr(va_arg(args, void *)));
	else if (c == 'x')
		return (prt_hexa(va_arg(args, ssize_t), false));
	else if (c == 'X')
		return (prt_hexa(va_arg(args, ssize_t), true));
	else if (c == 'i' || c == 'd')
		return (prt_int(va_arg(args, int)));
	else if (c == 's')
		return (prt_str(va_arg(args, char *)));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		len;
	va_list	args;

	if (!str)
		return (0);
	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			len += conversion(str[i], args);
		}
		else
			len += write (1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
