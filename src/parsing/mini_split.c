/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:56:19 by mdiederi          #+#    #+#             */
/*   Updated: 2025/06/30 09:56:22 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	**ft_is_valid_input(char const *str, char const *c)
{
    char	**ar;

    if (!str || str[0] == '\0')
    {
        ar = (char **)malloc(sizeof(char *));
        if (ar)
            ar[0] = NULL;
        return (ar);
    }
    while (*str)
    {
        if (!is_in_set(*str, c))
            return (NULL);
        str++;
    }
    ar = (char **)malloc(sizeof(char *));
    if (ar)
        ar[0] = NULL;
    return (ar);
}

static size_t	ft_get_ndelims(char const *str, char const *c)
{
    size_t	ndelims;

    ndelims = 0;
    if (!str)
        return (ndelims);
    while (is_in_set(*str, c))
        str++;
    while (*str)
    {
        if (is_in_set(*str, c))
        {
            ndelims++;
            while (is_in_set(*str, c))
                str++;
        }
        else
            str++;
    }
    if (ndelims > 0 && is_in_set(*(str - 1), c))
        ndelims--;
    return (ndelims);
}

static char	*ft_set_next_substr(char const *s, char const *c, size_t *str_ind)
{
    char	*substr;
    size_t	len_substr;
    size_t	start;

    start = 0;
    start += *str_ind;
    while (is_in_set(s[start], c))
        start++;
    len_substr = 0;
    while (s[start + len_substr] && !is_in_set(s[start + len_substr], c))
        len_substr++;
    *str_ind = len_substr + start;
    if (len_substr > 0)
    {
        substr = malloc(len_substr + 1);
        if (substr == NULL)
            return (NULL);
        ft_strlcpy(substr, &s[start], len_substr + 1);
        return (substr);
    }
    return (NULL);
}

char	**ft_split_on_str(char const *s, char const *c)
{
    char	**ar;
    size_t	i;
    size_t	str_ind;

    ar = ft_is_valid_input(s, c);
    if (ar)
        return (ar);
    ar = (char **)malloc((ft_get_ndelims((char *)s, c) + 2) * sizeof(char *));
    if (ar == NULL)
        return (NULL);
    str_ind = 0;
    i = 0;
    while (i < (ft_get_ndelims((char *)s, c) + 1))
    {
        ar[i] = ft_set_next_substr(s, c, &str_ind);
        if (ar[i] == NULL)
        {
            free_n_array(ar, i);
            return (NULL);
        }
        i++;
    }
    ar[i] = NULL;
    return (ar);
}