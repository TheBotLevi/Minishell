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

// Returns empty argv if only IFS and optionally redirs are present
// Otherwise NULL if there's any real non-redir, non-IFS token
static char	**ft_is_valid_input(t_token *str, t_token *end,
		int ignore_redirections)
{
	char	**ar;

	if (!str || str->c == '\0')
	{
		ar = (char **)malloc(sizeof(char *));
		if (ar)
			ar[0] = NULL;
		return (ar);
	}
	while (str && str != end)
	{
		if (str->is_ifs)
		{
			str = str->next;
			continue ;
		}
		if (ignore_redirections && (str->is_redirection
				|| str->is_redir_filename))
		{
			str = str->next;
			continue ;
		}
		return (NULL);
	}
	ar = (char **)malloc(sizeof(char *));
	if (ar)
		ar[0] = NULL;
	return (ar);
}

static size_t	ft_get_ndelims(t_token *str, t_token *end,
		int ignore_redirections)
{
	size_t	ndelims;
	t_token	*last;

	last = NULL;
	ndelims = 0;
	if (!str)
		return (0);
	while (str && str != end && (str->is_ifs || (ignore_redirections
				&& (str->is_redirection || str->is_redir_filename))))
		str = str->next;
	while (str && str != end)
	{
		if ( (!ignore_redirections && str->is_ifs)
	 || (ignore_redirections && !str->is_redirection && !str->is_redir_filename && str->is_ifs) )
		{
			ndelims++;
			while (str && str != end && (str->is_ifs || (ignore_redirections
				&& (str->is_redirection || str->is_redir_filename))))
				str = str->next;
		}
		else
		{
			last = str;
			str = str->next;
		}
	}
	if (last && last->next && last->next != end && (last->next->is_ifs
			|| (ignore_redirections && (last->next->is_redirection
					|| last->next->is_redir_filename))))
		ndelims--;
	return (ndelims);
}

static void	set_string_from_tokens(char *substr, t_token *start, t_token *stop) {
	int	i;

	i = 0;
	while (start)
	{
		if (!start->is_start_quote && !start->is_end_quote && !start->is_ifs)
			substr[i++] = start->c;
		if (start == stop)
			break;
		start = start->next;
	}
	substr[i] = '\0';
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
			break;
		start = start->next;
	}
	return (count);
}
static char *ft_set_next_substr(t_token **start, t_token *end, int ignore_redirections)
{
	char    *substr;
	size_t  len_substr;
	t_token *stop;
	t_token *stop_inclusive;
	int     n_quote_chars;

	// Skip leading IFS and (optionally) redirections
	while (*start && *start != end
		&& ((*start)->is_ifs || (ignore_redirections
		&& ((*start)->is_redirection || (*start)->is_redir_filename))))
		*start = (*start)->next;

	stop = *start;
	len_substr = 0;

	// Collect until IFS or redirection (when ignoring)
	while (stop && stop != end
		&& !(stop->is_ifs || (ignore_redirections
		&& (stop->is_redirection || stop->is_redir_filename))))
	{
		len_substr++;
		stop = stop->next;
	}

	// Determine inclusive stop token
	stop_inclusive = NULL;
	if (stop)
		stop_inclusive = stop->prev;
	else
		stop_inclusive = end;

	// Adjust for quotes
	n_quote_chars = count_quote_chars(*start, stop_inclusive);
	len_substr = len_substr - n_quote_chars;

	if (len_substr == 0 && n_quote_chars == 0)
		return (NULL);

	substr = malloc(len_substr + 1);
	if (!substr)
		return (NULL);

	set_string_from_tokens(substr, *start, stop_inclusive);
	*start = stop;
	if (stop)
		*start = stop->next;

	return (substr);
}
/*
static char	*ft_set_next_substr(t_token **start, t_token *end,
		int ignore_redirections)
{
	char	*substr;
	size_t	len_substr;
	t_token	*stop;
	int		n_quote_chars;

	while (*start && *start != end && ((*start)->is_ifs || (ignore_redirections
				&& ((*start)->is_redirection || (*start)->is_redir_filename))))
		*start = (*start)->next;
	stop = *start;
	len_substr = 0;
	while (stop && stop != end &&
	   !(stop->is_ifs || (ignore_redirections &&
	   (stop->is_redirection || stop->is_redir_filename))))
	{
		len_substr++;
		stop = stop->next;
	}
	n_quote_chars = count_quote_chars(*start, stop);
	len_substr = len_substr - n_quote_chars;
	if (len_substr == 0 && n_quote_chars == 0)
		return (NULL);
	substr = malloc(len_substr + 1);
	if (!substr)
		return (NULL);
	set_string_from_tokens(substr, *start, stop);
	*start = stop;
	if (stop)
		*start = stop->next;
	return (substr);
}*/

/* Split that accepts a string instead char as splitting param c,
	to treat the IFS string*/
char	**ft_split_on_ifs(t_token *start, t_token *end, int ignore_redirections)
{
	t_token	*curr;
	char	**ar;
	size_t	i;
	size_t	ndelims;

	curr = start;
	ar = ft_is_valid_input(curr, end, ignore_redirections);
	if (ar)
		return (ar);
	ndelims = ft_get_ndelims(curr, end, ignore_redirections);
	ar = (char **)malloc((ndelims + 2) * sizeof(char *));
	if (ar == NULL)
		return (NULL);
	i = 0;
	while (i < (ndelims + 1))
	{
		ar[i] = ft_set_next_substr(&curr, end, ignore_redirections);
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
