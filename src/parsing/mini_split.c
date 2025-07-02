/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:56:19 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/01 10:57:02 by ljeribha         ###   ########.fr       */
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
    /*int in_quote;
    t_quote_state state;*/

    ndelims = 0;
    if (!str)
        return (ndelims);
    while (is_in_set(*str, c))
        str++;
    /*in_quote = 0;
    memset(&state, 0, sizeof(t_quote_state));*/
    while (*str)
    {
       /* in_quote = is_within_quote(*str, &state);
        if (is_in_set(*str, c) && !in_quote)
        {
            ndelims++;
            while (is_in_set(*str, c))
                str++;
        }*/
        if (is_in_set(*str, c)) {
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

static char	*ft_set_next_substr(char const *s, char const *c, size_t *str_ind) {
    char	*substr;
    size_t	len_substr;
    size_t	start;
    //int *in_quote_arr;
    //t_quote_state state;

    start = 0;
    start += *str_ind;
    /*in_quote_arr = get_quote_state_array(s);
    if (in_quote_arr == NULL) //todo remove
        return (NULL);*/
    while (is_in_set(s[start], c))
        start++;
    len_substr = 0;
    //in_quote = 0;

    while ((s[start + len_substr] && !is_in_set(s[start + len_substr], c))) // || (s[start + len_substr] && is_within_quote(s[start + len_substr], &state)))
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

/* Split that accepts a string instead char as splitting param c, to treat the IFS string*/
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


/* determine the number of splits necessary to separate quotes from rest to know allocation size for array
 * for an array with len 1 or 2, no splits are possible:
 * len 1: no quote possible since only one char --> 0 splits
 * len 2: either all empty quote, or str --> 0 splits
 */
int get_n_splits(int *quote_arr, size_t arr_size) {
    size_t i;
    int n_splits;
    int prev_val;

    n_splits = 0;
    if (arr_size <= 2)
        return (n_splits);
    prev_val = quote_arr[0];
    i = 0;
    while (i < arr_size)
    {
        if (quote_arr[i] != prev_val)
            n_splits++;
        prev_val = quote_arr[i];
        i++;
    }
    return (n_splits);
}

void split_guarding_sep(char const *s, int *quote_arr, int arr_size, char **ar) {
    int char_ind;
    int array_ind;
    int str_ind;
    int prev_val;
    int start_val;
    int len;

    if (!s || !quote_arr || !ar || arr_size <= 0)
        return;

    char_ind = 0;
    array_ind = 0;
    str_ind = 0;
    prev_val = quote_arr[0];
    while (char_ind < arr_size) {
        if (quote_arr[char_ind] != prev_val) {
            len = 0;
            start_val = quote_arr[char_ind];
            int temp_char_ind = char_ind;
            while (temp_char_ind < arr_size && quote_arr[temp_char_ind] == start_val) {
                len++;
                temp_char_ind++;
            }

            ar[array_ind] = (char *)malloc((len + 1) * sizeof(char));
            if (ar[array_ind] == NULL) {
                free_n_array(ar, array_ind);
                return;
            }

            while (char_ind < arr_size && quote_arr[char_ind] == start_val) {
                ar[array_ind][str_ind] = s[char_ind];
                char_ind++;
                str_ind++;
            }
            ar[array_ind][str_ind] = '\0';
            array_ind++;
            str_ind = 0;
        }
        prev_val = quote_arr[char_ind];
        char_ind++;
    }
    ar[array_ind] = NULL;
}

/* Search for quotes and isolate them into their own array, leave the rest for
 * consecutive treatment in split on IFS
 * get pre, quotes and post arrays, split without eating the separators
 */
char	**split_quotes_comments(char const *s) {

    int n_splits;
    int *in_quote_arr;
    size_t int_arr_size;
    char **ar;

    in_quote_arr = get_quote_state_array(s);
    if (in_quote_arr == NULL)
        return (NULL);
    int_arr_size = get_int_array_size(in_quote_arr);
    n_splits = get_n_splits(in_quote_arr, int_arr_size+1);
    ar = (char **)malloc((n_splits + 2) * sizeof(char *));
    if (ar == NULL)
        return (NULL);
    if (n_splits == 0) {
        ar[0] = ft_strdup(s);
        ar[1] = NULL;
    }
    else
        split_guarding_sep(s, in_quote_arr, int_arr_size, ar);
    free(in_quote_arr);
    if (ar[0] == NULL)
        return (NULL);
    return (ar);
}

/*
void create_token_array(char **ar, t_mini *mini) {
    char **tokens;

}*/
