/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 07:59:17 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/29 14:43:34 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*end;
	char		*trimmed_str;
	size_t		len;

	if (!s1 || !set)
		return (0);
	end = s1 + ft_strlen(s1) - 1;
	while (*s1 && is_in_set(*s1, set))
	{
		s1++;
	}
	while (end > s1 && is_in_set(*end, set))
	{
		end--;
	}
	len = end - s1 + 1;
	if (len == 0)
		return (ft_strdup(""));
	trimmed_str = malloc(len + 1);
	if (!trimmed_str)
		return (NULL);
	ft_strncpy(trimmed_str, s1, len);
	trimmed_str[len] = '\0';
	return (trimmed_str);
}

/*int	main()
{
	char	*str = ft_strtrim("aaaaaaaaCHEEEEEFaaaaaaaaa", "a");
	if (str)
	{
		printf("The trimmed string: %s\n", str);
		free(str);
	}
	return (0);
}*/