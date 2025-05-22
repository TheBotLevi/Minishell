/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:56:43 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 09:16:22 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(long nbr);
static char	*pre_conv(int len);

static int	int_len(long nbr)
{
	int	count;

	count = 0;
	if (nbr < 0)
	{
		count++;
		nbr = -nbr;
	}
	if (nbr == 0)
		count++;
	while (nbr != 0)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (0);
	tmp[len] = '\0';
	return (tmp);
}

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	nbr;

	nbr = n;
	len = int_len(nbr);
	result = pre_conv(len);
	if (!result)
		return (0);
	i = len - 1;
	if (nbr == 0)
		result[0] = '0';
	if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr != 0)
	{
		result[i] = (nbr % 10) + '0';
		nbr = nbr / 10;
		i--;
	}
	return (result);
}

/*int     main()
{
	int	n = -12345;
        char    *str = ft_itoa(n);
        if (str)
	{
		printf("The integer as a string: %s\n", str);
		free(str);
	}
	return (0);
}*/