/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_quotes.c                                      :+:      :+:    :+:   */
/*   mini_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:56:34 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/02 19:56:39 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t get_int_array_size(const int *arr) {
    size_t size;

    size = 0;
    if (!arr)
        return (0);
    while (arr[size] != -1)
        size++;
    return (size);
}

/* state keeps track of already being in a quote and which type (single or double)
 * IF used in a loop and the last element is still marked as "within_quote" it
 * indicates an unfinished quote
 */
int is_within_quote(const char c, t_quote_state *state) {

    if (!state->within_quote && is_in_set(c, "\'\"")) {
        state->within_quote = 1;
        if (c == '\'')
            state->in_single_quote = 1;
        if (c == '\"')
            state->in_double_quote = 1;
        return (1);
    }
    if (c == '\'' && state->in_single_quote) {
        state->within_quote = 0;
        state->in_single_quote = 0;
        return (1);
    }
    if (c == '\"' && state->in_double_quote) {
        state->within_quote = 0;
        state->in_double_quote = 0;
        return (1);
    }
    if (state->within_quote)
        return (1);
    return (0);
}

/* sets last detected quote of the quote_array back to zero as quote was not
 * correctly terminated and should be interpreted as literal string*/
void cancel_unfinished_quote(int *in_quote_arr, int last_elem_idx) {

    while (in_quote_arr && last_elem_idx >= 0 && in_quote_arr[last_elem_idx] == 1) {
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

    in_quote_arr = (int *)malloc(sizeof(int) * (ft_strlen(str) + 1));
    if (in_quote_arr == NULL)
        return (NULL);
    ft_memset(in_quote_arr, 0, sizeof(int) * (ft_strlen(str) + 1));
    ft_memset(&state, 0, sizeof(t_quote_state));
    i = 0;
    while (str[i] != '\0') {
        in_quote_arr[i] = is_within_quote(str[i], &state);
        printf("char: %c, within_quote: %d, single: %d, double: %d\n", str[i], state.within_quote, state.in_single_quote, state.in_double_quote);
        i++;
    }
    if (state.within_quote == 1)
        cancel_unfinished_quote(in_quote_arr, i-1);
    in_quote_arr[i] = -1;
    return (in_quote_arr);
}
