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

typedef struct s_quote_state {
    int in_single_quote;
    int in_double_quote;
    int within_quote;
    //todo track unfinished quotes and treat as normal string
} t_quote_state;

/* state keeps track of already being in a quote and which type (single or double)
 * IF used in a loop and the last element is still marked as "within_quote" it
 * indicates an unfinished quote
 */
int is_within_quote(char c, t_quote_state *state) {

    if (!state->within_quote && (c == '\'' || c == '\"')) {
        state->within_quote = 1;
        if (c == '\'')
            state->in_single_quote = 1;
        if (c == '\"')
            state->in_double_quote = 1;
        return (1); // change to 1 to include quote mark
    }
    if (state->within_quote && c == '\'' && state->in_single_quote) {
        state->within_quote = 0;
        state->in_single_quote = 0;
        return (1); // change to 1 to include quote mark
    }
    if (state->within_quote && c == '\"' && state->in_double_quote) {
        state->within_quote = 0;
        state->in_double_quote = 0;
        return (1); // change to 1 to include quote mark
    }
    if (state->within_quote)
        return (1);
    return (0);
}

/* sets last detected quote of the quote_array back to zero as quote was not
 * correctly terminated and should be interpreted as literal string*/
void cancel_unfinished_quote(int *in_quote_arr, int last_elem_idx) {

    while (in_quote_arr[last_elem_idx] == 1 && last_elem_idx >= 0) {
        in_quote_arr[last_elem_idx] = 0;
        last_elem_idx--;
    }
}

/* goes through string and detects single and double quotes, returning
// a -1 terminted int array indicating whether the character is wihtin a
quoted string (incl. its quotation marks) */
int *get_quote_state_array(char const *str)
{
    int *in_quote_arr;
    t_quote_state state;
    int i;

    in_quote_arr = (int *)malloc(sizeof(int)*ft_strlen(str)+1);
    if (in_quote_arr == NULL)
        return (NULL);
    i = 0;
    while (str[i] != '\0') {
        in_quote_arr[i] = is_within_quote(str[i], &state);
        i++;
    }
    if (state.within_quote == 1)
        cancel_unfinished_quote(in_quote_arr, i-1);
    in_quote_arr[i] = -1;
    return (in_quote_arr);
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

size_t get_int_array_size(const int *arr) {
    int i;
    size_t size;

    i = 0;
    while (arr[i] != -1)
    {
        size++;
        i++;
    }
    return (size);
}

/* determine the number of splits necessary to separate quotes from rest to know allocation size for array
 * for an array with len 1 or 2, no splits are possible:
 * len 1: no quote possible since only one char --> 0 splits
 * len 2: either all empty quote, or str --> 0 splits
 */
int get_n_splits(int *quote_arr, int arr_size) {
    int i;
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
}

char** split_guarding_sep(char const *s, int *quote_arr, int arr_size, int n_splits) {
    char **ar;
    int char_ind;
    int array_ind;
    int str_ind;
    int prev_val;
    int start_val;

    ar = (char **)malloc((n_splits + 2) * sizeof(char *));
    if (ar == NULL)
        return (NULL);
    char_ind = 0;
    array_ind = 0;
    str_ind = 0;
    prev_val = quote_arr[0];
    while (char_ind < arr_size)
    {
        if (quote_arr[char_ind] != prev_val) {
            start_val = quote_arr[char_ind];
            while (quote_arr[char_ind] == start_val) {
                ar[array_ind][str_ind] = s[char_ind];
                char_ind++;
                str_ind++;
            }
            ar[array_ind][str_ind] = '\0';
            array_ind++;
            str_ind = 0;
            if (quote_arr[char_ind] != prev_val) {
                ar[array_ind] = ft_set_next_substr(s, get_ifs_from_env(mini), &char_ind);
                if (ar[array_ind] == NULL)
                {
                    free_n_array(ar, array_ind);
                    return (NULL);
                }
            }
            }
        }
        prev_val = quote_arr[char_ind];
        char_ind++;
        if (quote_arr[char_ind] == 1)
            s++;
            char_ind++;
        else if (quote_arr[char_ind] == 0)
        {
            ar[array_ind] = ft_set_next_substr(s, get_ifs_from_env(mini), &char_ind);
            if (ar[array_ind] == NULL)
            {
                free_n_array(ar, array_ind);
                return (NULL);
            }
            array_ind++;
        }
    }

}

/* Search for quotes and isolate them into their own array, leave the rest for consecutive treatment in split on IFS*/
char	**split_quotes_comments(char const *s, t_mini *mini) {

    int n_splits;
    int *in_quote_arr;
    int int_arr_size;
    char **ar;
    size_t i;
    size_t str_ind;

    in_quote_arr = get_quote_state_array(s);
    if (in_quote_arr == NULL)
        return (NULL);

    int_arr_size = get_int_array_size(in_quote_arr);
    n_splits = get_n_splits(in_quote_arr, int_arr_size);
    if (n_splits == 0)
        return (ft_split_on_str(s, get_ifs_from_env(mini)));

    //get pre, quotes and post arrays, split without eating the separators
    ar = split_guarding_sep(s, in_quote_arr, int_arr_size, n_splits);
    if (ar == NULL)
        return (NULL);
    free(in_quote_arr);

}