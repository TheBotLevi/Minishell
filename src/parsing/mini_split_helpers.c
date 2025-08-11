/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:11:07 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/11 21:11:10 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_skip_token(t_token *t, int ignore_redirections)
{
	return (t->is_ifs || (ignore_redirections && (t->is_redirection
				|| t->is_redir_filename)));
}

static int	is_delim_token(t_token *t, int ignore_redirections)
{
	return (t->is_ifs && (!ignore_redirections || (!t->is_redirection
				&& !t->is_redir_filename)));
}

size_t	advance_and_count_delims(t_token *str, t_token *end, t_token **last,
		int ignore_redirections)
{
	size_t	ndelims;

	ndelims = 0;
	while (str && str != end && is_skip_token(str, ignore_redirections))
		str = str->next;
	while (str && str != end)
	{
		if (is_delim_token(str, ignore_redirections))
		{
			ndelims++;
			while (str && str != end && is_skip_token(str, ignore_redirections))
				str = str->next;
		}
		else
		{
			*last = str;
			str = str->next;
		}
	}
	return (ndelims);
}

int	count_quote_chars(const t_token *start, const t_token *end)
{
	int	count;

	count = 0;
	while (start)
	{
		if (start->is_start_quote || start->is_end_quote)
			count++;
		if (start == end)
			break ;
		start = start->next;
	}
	return (count);
}

// Skip leading IFS and (optionally) redirections
// Collect until IFS or redirection (when ignoring)
size_t	set_start_stop(t_token **start, t_token **stop, t_token *end,
		int ignore_redirections)
{
	size_t	len_substr;

	len_substr = 0;
	while (*start && *start != end && ((*start)->is_ifs || (ignore_redirections
				&& ((*start)->is_redirection || (*start)->is_redir_filename))))
		*start = (*start)->next;
	*stop = *start;
	while ((*stop) && (*stop) != end && !((*stop)->is_ifs
			|| (ignore_redirections && ((*stop)->is_redirection
					|| (*stop)->is_redir_filename))))
	{
		len_substr++;
		*stop = (*stop)->next;
	}
	return (len_substr);
}
