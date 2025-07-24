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

/* state keeps track of already being in a quote and which type (single or double)
 * IF used in a loop and the last element is still marked as "within_quote" it
 * indicates an unfinished quote
 */
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
	if (state->within_quote)
	{
		state->is_start_quote = 0;
		return (1);
	}
	return (0);
}

/* sets last detected quote of the quote_array back to zero as quote was not
 * correctly terminated and should be interpreted as literal string*/
void	cancel_unfinished_quote_token(t_token *token)
{
	t_token	*current;

	if (!token)
		return ;
	current = token;
	while (current && current->is_quote && !current->is_start_quote)
	{
		current->is_quote = 0;
		current->is_double_quote = 0;
		current->is_single_quote = 0;
		current->is_start_quote = 0;
		current = current->prev;
	}
	if (current && current->is_start_quote)
	{
		current->is_quote = 0;
		current->is_double_quote = 0;
		current->is_single_quote = 0;
		current->is_start_quote = 0;
	}
}

/*set comment flag till EOL after encountering first unquoted #*/
void	mark_comment(t_token *tokens)
{
	t_token	*current;
	int		in_comment;

	in_comment = 0;
	current = tokens;
	while (current)
	{
		if (current->is_quote == 0 && current->c == '#')
		{
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

/* goes through string and detects single and double quotes,
	setting the correspondig token flags.
 * when last token has been reached,
	it exits without incrementing to check if a quote remains unfinished
//  */
int	set_quote_flags(t_token *tokens)
{
	t_quote_state	state;
	t_token			*current;

	if (!tokens)
		return (1);
	ft_memset(&state, 0, sizeof(t_quote_state));
	current = tokens;
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
			cancel_unfinished_quote_token(current);
		current = current->next;
	}
	return (0);
}
