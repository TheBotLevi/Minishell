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

/*ensures that after unquoted #, rest of line is ignored by ending the int array
 *through setting the rest to -1*/
void cancel_non_quote_comment(char const *str, int *in_quote_arr) {
    int i;

    i = 0;
    while (in_quote_arr[i] != -1) {
        if (in_quote_arr[i] == 0 && str[i] == '#') {
            while (in_quote_arr[i] != -1) {
                in_quote_arr[i] = -1;
                i++;
            }
            break;
        }
        i++;
    }
}

/* goes through string and detects single and double quotes, returning
// a -1 terminted int array indicating whether the character is wihtin a
quoted string (incl. its quotation marks) or comment */
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
        //printf("char: %c, within_quote: %d, single: %d, double: %d\n", str[i], state.within_quote, state.in_single_quote, state.in_double_quote);
        i++;
    }
    if (state.within_quote == 1)
        cancel_unfinished_quote(in_quote_arr, i-1);
    in_quote_arr[i] = -1;
    cancel_non_quote_comment(str, in_quote_arr);
    i = 0;
    printf("in_quote_arr: ");
    while (in_quote_arr[i] != -1)
        printf("%d ", in_quote_arr[i++]);
    printf("\n");
    return (in_quote_arr);
}

// ################################################## token version

/* state keeps track of already being in a quote and which type (single or double)
 * IF used in a loop and the last element is still marked as "within_quote" it
 * indicates an unfinished quote
 */
int is_within_quote_token(const char c, t_quote_state *state) {

    state->is_end_quote = 0;
    if (!state->within_quote && is_in_set(c, "\'\"")) {
        state->within_quote = 1;
        state->is_start_quote = 1;
        if (c == '\'')
            state->in_single_quote = 1;
        if (c == '\"')
            state->in_double_quote = 1;
        return (1);
    }
    if (c == '\'' && state->in_single_quote) {
        state->is_start_quote = 0;
        state->is_end_quote = 1;
        state->within_quote = 0;
        state->in_single_quote = 0;
        return (1);
    }
    if (c == '\"' && state->in_double_quote) {
        state->is_start_quote = 0;
        state->is_end_quote = 1;
        state->within_quote = 0;
        state->in_double_quote = 0;
        return (1);
    }
    if (state->within_quote) {
        state->is_start_quote = 0;
        return (1);
    }
    return (0);
}

/* sets last detected quote of the quote_array back to zero as quote was not
 * correctly terminated and should be interpreted as literal string*/
void cancel_unfinished_quote_token(t_token *token) {
    t_token *current;

    if (!token)
        return;
    current = token;
    while (current && current->is_quote) {
        current->is_quote = 0;
        current->is_double_quote = 0;
        current->is_single_quote = 0;
        current->is_start_quote = 0;
        current = current->prev;
        if (current && current->is_start_quote) {
            current->is_quote = 0;
            current->is_double_quote = 0;
            current->is_single_quote = 0;
            current->is_start_quote = 0;
            break;
        }
    }
}

/*set comment flag till EOL after encountering first unquoted #*/
void mark_comment(t_token **tokens) {
    t_token *current;
    int in_comment;

    in_comment = 0;
    current = *tokens;
    while (current) {
        if (current->is_quote == 0 && current->c == '#') {
            current->is_comment_start = 1;
            in_comment = 1;
        }
        if (in_comment)
            current->is_comment = 1;
            //todo see if helpful to erase quote info (since overriden by comment)
           /* current->is_quote = 0;
            current->is_double_quote = 0;
            current->is_single_quote = 0;
            current->is_start_quote = 0;
            current->is_end_quote = 0;*/
       current = current->next;
    }
}

/* goes through string and detects single and double quotes, returning
// a -1 terminated int array indicating whether the character is wihtin a
quoted string (incl. its quotation marks) or comment */
int set_quote_flags(t_token **tokens) {
    t_quote_state state;
    t_token *current;

    if (!tokens)
        return (1);
    ft_memset(&state, 0, sizeof(t_quote_state));
    current = *tokens;
    while (current) {
        current->is_quote = is_within_quote_token(current->c, &state);
        current->is_double_quote = state.in_double_quote;
        current->is_single_quote = state.in_single_quote;
        current->is_start_quote = state.is_start_quote;
        current->is_end_quote = state.is_end_quote;
        if (current->is_end_quote && current->prev) {
            current->is_double_quote = current->prev->is_double_quote;
            current->is_single_quote = current->prev->is_single_quote;
        }
        if (current->next)
            current = current->next;
        else
            break;
    }
    if (state.within_quote == 1)
        cancel_unfinished_quote_token(current);
    return (0);
}
