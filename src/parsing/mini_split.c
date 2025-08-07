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

// if str is not just delimiter, return null, else return empty array
static char	**ft_is_valid_input(t_token *str, t_token *end)
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
		if (!str->is_ifs)
			return (NULL);
		str = str->next;
	}
	ar = (char **)malloc(sizeof(char *));
	if (ar)
		ar[0] = NULL;
	return (ar);
}

static size_t	ft_get_ndelims(t_token *str, t_token *end)
{
	size_t	ndelims;
	t_token	*last;

	last = NULL;
	ndelims = 0;
	if (!str)
		return (ndelims);
	while (str && str != end && str->is_ifs)
		str = str->next;
	while (str && str != end)
	{
		if (str->is_ifs)
		{
			ndelims++;
			while (str && str != end && str->is_ifs)
				str = str->next;
		}
		else {
			last = str;
			str = str->next;
		}
	}
	/*if (ndelims > 0 && str && str->prev && str->prev->is_ifs)
		ndelims--;*/
	if (last && last->next && last->next != end && last->next->is_ifs)
		ndelims--;
	return (ndelims);
}

static void	set_string_from_tokens(char *substr, t_token *start, t_token *stop, int is_fully_quoted)
{
	int i;

	i = 0;
	if (is_fully_quoted && start->is_start_quote)
		start = start->next;
	while (start && start != stop) {
		substr[i] = start->c;
		start = start->next;
		i++;
		if (start == stop && start && start->is_end_quote && is_fully_quoted)
			break;
	}
	substr[i] = '\0';
}

int check_for_fully_quoted_str(const t_token *start, const t_token *end) {

	if (!start || !start->is_start_quote)
		return (0);
	while (start && start->is_quote && !start->is_end_quote && start != end)
		start = start->next;
	if (start == end && start && start->is_quote && start->is_end_quote)
		return (1);
	return (0);
}

static char	*ft_set_next_substr(t_token **start, t_token *end)
{
	char	*substr;
	size_t	len_substr;
	t_token	*stop;
	int		is_fully_quoted;

	stop = *start;
	while (stop && stop != end && stop->is_ifs)
		stop = stop->next;
	len_substr = 0;
	*start = stop;
	while (stop && stop != end && !stop->is_ifs) {
		len_substr++;
		stop = stop->next;
	}
	is_fully_quoted = check_for_fully_quoted_str(*start, end);
	if (is_fully_quoted)
		len_substr = len_substr - 2;
	if (len_substr > 0)
	{
		substr = malloc(len_substr + 1);
		if (substr == NULL)
			return (NULL);
		set_string_from_tokens(substr, *start, stop, is_fully_quoted);
		if (stop)
			printf("substr: %s, *start: %c, stop: %c, is_fully_quoted: %d\n", substr, (*start)->c, stop->c, is_fully_quoted);
		else
			printf("substr: %s, *start: %c, stop: NULL EOL, is_fully_quoted: %d\n", substr, (*start)->c, is_fully_quoted);
		*start = stop;
		return (substr);
	}
	return (NULL);
}


/* Split that accepts a string instead char as splitting param c,
	to treat the IFS string*/
char	**ft_split_on_ifs(t_token *start, t_token *end)
{
	t_token	*curr;
	char	**ar;
	size_t	i;
	size_t	ndelims;

	curr = start;
	ar = ft_is_valid_input(curr, end);
	if (ar)
		return (ar);
	ndelims = ft_get_ndelims(curr, end);
	ar = (char **)malloc((ndelims + 2) * sizeof(char *));
	if (ar == NULL)
		return (NULL);
	i = 0;
	while (i < (ndelims + 1))
	{
		ar[i] = ft_set_next_substr(&curr, end);
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
