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
    i = 1;
    while (i < (arr_size-1))
    {
        if (quote_arr[i] != prev_val)
            n_splits++;
        prev_val = quote_arr[i];
        i++;
    }
    return (n_splits);
}

int *get_sizes_arr(const int *quote_arr, const size_t arr_size, const int n_splits) {
    size_t i;
    int j;
    int n_array;
    int *size_arr;
    int prev_val;

    size_arr = malloc(sizeof(size_t) * (n_splits + 2));
    if (!size_arr || !quote_arr || arr_size == 0 || n_splits < 0)
        return NULL;
    prev_val = quote_arr[0];
    i = 1;
    j = 0;
    n_array = 1;
    while (i < arr_size) {
        if (quote_arr[i] != prev_val) {
            size_arr[j++] = n_array;
            n_array = 0;
        }
        prev_val = quote_arr[i];
        n_array++;
        i++;
    }
    size_arr[j++] = n_array;
    size_arr[j] = -1;
    return (size_arr);
}

void finish_array(char**ar, int* array_ind, int* str_ind) {
    ar[*array_ind][*str_ind] = '\0';
    (*array_ind)++;
    *str_ind = 0;
}

void split_guarding_sep(t_tok_data *tok_data) {
    int char_ind;
    int array_ind;
    int str_ind;

    if (!tok_data->line || !tok_data->in_quote_arr || !tok_data->ar || tok_data->n_elems <= 0)
        return;
    char_ind = 0;
    array_ind = 0;
    str_ind = 0;
    while (array_ind < tok_data->n_splits + 1) {
        tok_data->ar[array_ind] = (char *)malloc((tok_data->size_arr[array_ind] + 1) * sizeof(char));
        if (tok_data->ar[array_ind] == NULL) {
            free_n_array(tok_data->ar, array_ind);
            return;
        }
        while (str_ind < tok_data->size_arr[array_ind]) {
            tok_data->ar[array_ind][str_ind] = tok_data->line[char_ind];
            char_ind++;
            str_ind++;
        }
        finish_array(tok_data->ar, &array_ind, &str_ind);
        }
    tok_data->ar[array_ind] = NULL;
    }


t_tok_data* init_tok_data(char const *line) {
    t_tok_data *tok_data;

    tok_data = malloc(sizeof(t_tok_data));
    if (!tok_data)
        return (NULL);
    tok_data->line = line;
    tok_data->n_elems = ft_strlen(line);
    tok_data->in_quote_arr = get_quote_state_array(line);
    if (tok_data->in_quote_arr == NULL) {
        free_tok_data(tok_data);
        return (NULL);
    }
    tok_data->n_splits = get_n_splits(tok_data->in_quote_arr, tok_data->n_elems+1);
    tok_data->size_arr = get_sizes_arr(tok_data->in_quote_arr, tok_data->n_elems+1, tok_data->n_splits);
    if (tok_data->size_arr == NULL) {
        free_tok_data(tok_data);
        return (NULL);
    }
    tok_data->ar = (char **)malloc((tok_data->n_splits + 2) * sizeof(char *));
    if (tok_data->ar == NULL) {
        free_tok_data(tok_data);
        return (NULL);
    }
    return(tok_data);
}

void free_tok_data(t_tok_data *tok_data){
    if (!tok_data)
        return;
    free(tok_data->in_quote_arr);
    tok_data->in_quote_arr = NULL;
    free(tok_data->size_arr);
    tok_data->size_arr = NULL;
    free_args(tok_data->ar);
    free(tok_data);
    tok_data = NULL;
}

/* Search for quotes and isolate them into their own array, leave the rest for
 * consecutive treatment in split on IFS
 * get pre, quotes and post arrays, split without eating the separators
 */
t_tok_data *split_quotes_comments(char const *line) {
    t_tok_data *tok_data;

    tok_data = init_tok_data(line);
    if (!tok_data)
        return (NULL);
    printf("n_splits: %d\n", tok_data->n_splits);
    if (tok_data->n_splits == 0) {
        tok_data->ar[0] = ft_strdup(line);
        if (tok_data->ar[0] == NULL) {
            return NULL;
        }
        tok_data->ar[1] = NULL;
    }
    else
        split_guarding_sep(tok_data);
    if (tok_data->ar[0] == NULL){
        free_tok_data(tok_data);
        return (NULL);
    }
    return (tok_data);
}

/*
void create_token_array(char **ar, t_mini *mini) {
    char **tokens;

}*/
