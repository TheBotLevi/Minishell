/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:17:15 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 09:41:59 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*new_str;
	size_t			len;

	len = ft_strlen(s);
	if (!s || !f)
		return (0);
	new_str = malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (0);
	i = 0;
	while (s[i])
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

/*char	sample_function(unsigned int i, char c)
{
	return (c + i % 26);
}

int	main()
{
	char	*result = ft_strmapi("abcdef", sample_function);
	if (result)
	{
		printf("Result: %s\n", result);
		free(result);
	}
	return (0);
}*/