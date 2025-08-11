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
		if (str->is_ifs || (ignore_redirections && (str->is_redirection
					|| str->is_redir_filename)))
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

// skip initial skip-tokens
// adjust if last token is followed by a skip-token
static size_t	ft_get_ndelims(t_token *str, t_token *end,
		int ignore_redirections)
{
	size_t	ndelims;
	t_token	*last;

	last = NULL;
	if (!str)
		return (0);
	ndelims = advance_and_count_delims(str, end, &last, ignore_redirections);
	if (last && last->next && last->next != end && is_skip_token(last->next,
			ignore_redirections))
		ndelims--;
	return (ndelims);
}

static void	set_string_from_tokens(char *substr, t_token *start, t_token *stop)
{
	int	i;

	i = 0;
	while (start)
	{
		if (!start->is_start_quote && !start->is_end_quote && !start->is_ifs)
			substr[i++] = start->c;
		if (start == stop)
			break ;
		start = start->next;
	}
	substr[i] = '\0';
}

// Determine inclusive stop token
// Adjust for quotes
static char	*ft_set_next_substr(t_token **start, t_token *end,
		int ignore_redirections)
{
	char	*substr;
	size_t	len_substr;
	t_token	*stop;
	t_token	*stop_inclusive;
	int		n_quote_chars;

	len_substr = set_start_stop(start, &stop, end, ignore_redirections);
	stop_inclusive = NULL;
	if (stop)
		stop_inclusive = stop->prev;
	else
		stop_inclusive = end;
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

/* Split that accepts a string instead char as splitting param c,
	to treat the IFS string*/
char	**ft_split_on_ifs(t_token *start, t_token *end, int ignore_redirections)
{
	char	**ar;
	size_t	i;
	size_t	ndelims;

	ar = ft_is_valid_input(start, end, ignore_redirections);
	if (ar)
		return (ar);
	ndelims = ft_get_ndelims(start, end, ignore_redirections);
	ar = (char **)malloc((ndelims + 2) * sizeof(char *));
	if (ar == NULL)
		return (NULL);
	i = 0;
	while (i < (ndelims + 1))
	{
		ar[i] = ft_set_next_substr(&start, end, ignore_redirections);
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
