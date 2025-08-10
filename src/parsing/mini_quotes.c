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

int	check_finish_quote(const char c, t_quote_state *state)
{
	if (c == '\'' && state->in_single_quote)
	{
		state->is_start_quote = 0;
		state->is_end_quote = 1;
		state->within_quote = 0;
		state->in_single_quote = 0;
		return (1);
	}
	if (c == '\"' && state->in_double_quote)
	{
		state->is_start_quote = 0;
		state->is_end_quote = 1;
		state->within_quote = 0;
		state->in_double_quote = 0;
		return (1);
	}
	return (0);
}

/* state keeps track of already being in a quote and which type (single or
 * double) IF used in a loop and the last element is still marked as
 * "within_quote" it indicates an unfinished quote */
int	is_within_quote_token(const char c, t_quote_state *state)
{
	state->is_end_quote = 0;
	if (!state->within_quote && is_in_set(c, "\'\""))
	{
		state->within_quote = 1;
		state->is_start_quote = 1;
		if (c == '\'')
			state->in_single_quote = 1;
		if (c == '\"')
			state->in_double_quote = 1;
		return (1);
	}
	if (check_finish_quote(c, state))
		return (1);
	if (state->within_quote)
	{
		state->is_start_quote = 0;
		return (1);
	}
	return (0);
}

/* goes through string and detects single and double quotes,
	setting the correspondig token flags.
 * when last token has been reached,
	it exits without incrementing to check if a quote remains unfinished

	returns 1 if quote is unfinished
  */
int	mark_quote_flags(t_token *current)
{
	t_quote_state	state;

	if (!current)
		return (0);
	ft_memset(&state, 0, sizeof(t_quote_state));
	while (current)
	{
		current->is_quote = is_within_quote_token(current->c, &state);
		current->is_double_quote = state.in_double_quote;
		current->is_single_quote = state.in_single_quote;
		current->is_start_quote = state.is_start_quote;
		current->is_end_quote = state.is_end_quote;
		if (current->is_end_quote && current->prev)
		{
			current->is_double_quote = current->prev->is_double_quote;
			current->is_single_quote = current->prev->is_single_quote;
		}
		if (!current->next && state.within_quote == 1)
			return (1);
		current = current->next;
	}
	return (0);
}
