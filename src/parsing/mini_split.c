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
	if (last && last->next && last->next != end && last->next->is_ifs)
		ndelims--;
	return (ndelims);
}

static void	set_string_from_tokens(char *substr, t_token *start, t_token *stop)
{
	int i;

	i = 0;
	while (start && start != stop) {
		if (!start->is_start_quote && !start->is_end_quote) {
			substr[i] = start->c;
			i++;
		}
		start = start->next;
	}
	substr[i] = '\0';
}

int count_quote_chars(const t_token *start, const t_token *end) {
	int count = 0;

	count = 0;
	while (start && start != end){
		if (start->is_start_quote || start->is_end_quote)
			count++;
		start = start->next;
	}
	return (count);
}

static char	*ft_set_next_substr(t_token **start, t_token *end)
{
	char	*substr;
	size_t	len_substr;
	t_token	*stop;

	stop = *start;
	while (stop && stop != end && stop->is_ifs)
		stop = stop->next;
	len_substr = 0;
	*start = stop;
	while (stop && stop != end && !stop->is_ifs) {
		len_substr++;
		stop = stop->next;
	}
	len_substr = len_substr - count_quote_chars(*start, stop);
	if (len_substr > 0)
	{
		substr = malloc(len_substr + 1);
		if (substr == NULL)
			return (NULL);
		set_string_from_tokens(substr, *start, stop);
		if (stop)
			printf("substr: %s, *start: %c, stop: %c, n_quote_chars: %d\n", substr, (*start)->c, stop->c, count_quote_chars(*start, stop));
		else
			printf("substr: %s, *start: %c, stop: NULL EOL, n_quote_chars: %d\n", substr, (*start)->c, count_quote_chars(*start, stop));
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
