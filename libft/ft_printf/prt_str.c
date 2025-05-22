/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prt_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:36:03 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:19:27 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	prt_str(const char *str)
{
	if (str == NULL)
		return (write (1, "(null)", 6));
	else
		return (write (1, str, ft_strlen(str)));
}
