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

size_t	get_int_array_size(const int *arr)
{
	size_t	size;

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
int	is_within_quote(const char c, t_quote_state *state)
{
	if (!state->within_quote && is_in_set(c, "\'\""))
	{
		state->within_quote = 1;
		if (c == '\'')
			state->in_single_quote = 1;
		if (c == '\"')
			state->in_double_quote = 1;
		return (1);
	}
	if (c == '\'' && state->in_single_quote)
	{
		state->within_quote = 0;
		state->in_single_quote = 0;
		return (1);
	}
	if (c == '\"' && state->in_double_quote)
	{
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
void	cancel_unfinished_quote(int *in_quote_arr, int last_elem_idx)
{
	while (in_quote_arr && last_elem_idx >= 0
		&& in_quote_arr[last_elem_idx] == 1)
	{
		in_quote_arr[last_elem_idx] = 0;
		last_elem_idx--;
	}
}

/*ensures that after unquoted #, rest of line is ignored by ending the int array
 *through setting the rest to -1*/
void	cancel_non_quote_comment(char const *str, int *in_quote_arr)
{
	int	i;

	i = 0;
	while (in_quote_arr[i] != -1)
	{
		if (in_quote_arr[i] == 0 && str[i] == '#')
		{
			while (in_quote_arr[i] != -1)
			{
				in_quote_arr[i] = -1;
				i++;
			}
			break ;
		}
		i++;
	}
}

/* goes through string and detects single and double quotes, returning
// a -1 terminted int array indicating whether the character is wihtin a
quoted string (incl. its quotation marks) or comment */
int	*get_quote_state_array(char const *str)
{
	int				*in_quote_arr;
	t_quote_state	state;
	int				i;

	in_quote_arr = (int *)malloc(sizeof(int) * (ft_strlen(str) + 1));
	if (in_quote_arr == NULL)
		return (NULL);
	ft_memset(in_quote_arr, 0, sizeof(int) * (ft_strlen(str) + 1));
	ft_memset(&state, 0, sizeof(t_quote_state));
	i = 0;
	while (str[i] != '\0')
	{
		in_quote_arr[i] = is_within_quote(str[i], &state);
		/*printf("char: %c, within_quote: %d, single: %d, double: %d\n", str[i],
				state.within_quote, state.in_single_quote,
				state.in_double_quote);*/
		i++;
	}
	if (state.within_quote == 1)
		cancel_unfinished_quote(in_quote_arr, i - 1);
	in_quote_arr[i] = -1;
	cancel_non_quote_comment(str, in_quote_arr);
	i = 0;
	printf("in_quote_arr: ");
	while (in_quote_arr[i] != -1)
		printf("%d ", in_quote_arr[i++]);
	printf("\n");
	return (in_quote_arr);
}

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

	ndelims = 0;
	if (!str)
		return (ndelims);
	while (is_in_set(*str, c))
		str++;
	while (*str)
	{
		if (is_in_set(*str, c))
		{
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

static char	*ft_set_next_substr(char const *s, char const *c, size_t *str_ind)
{
	char	*substr;
	size_t	len_substr;
	size_t	start;

	start = 0;
	start += *str_ind;
	while (is_in_set(s[start], c))
		start++;
	len_substr = 0;
	while ((s[start + len_substr] && !is_in_set(s[start + len_substr], c)))
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

/* Split that accepts a string instead char as splitting param c,
	to treat the IFS string*/
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
int	get_n_splits(int *quote_arr, size_t arr_size)
{
	size_t	i;
	int		n_splits;
	int		prev_val;

	n_splits = 0;
	if (arr_size <= 2)
		return (n_splits);
	prev_val = quote_arr[0];
	i = 1;
	while (i < (arr_size - 1))
	{
		if (quote_arr[i] != prev_val)
			n_splits++;
		prev_val = quote_arr[i];
		i++;
	}
	return (n_splits);
}

int	*get_sizes_arr(const int *quote_arr, const size_t arr_size,
		const int n_splits)
{
	size_t	i;
	int		j;
	int		n_array;
	int		*size_arr;
	int		prev_val;

	size_arr = malloc(sizeof(size_t) * (n_splits + 2));
	if (!size_arr || !quote_arr || arr_size == 0 || n_splits < 0)
		return (NULL);
	prev_val = quote_arr[0];
	i = 1;
	j = 0;
	n_array = 1;
	while (i < arr_size)
	{
		if (quote_arr[i] != prev_val)
		{
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

void	finish_array(char **ar, int *array_ind, int *str_ind)
{
	ar[*array_ind][*str_ind] = '\0';
	(*array_ind)++;
	*str_ind = 0;
}

void	split_guarding_sep(t_tok_data *tok_data)
{
	int	char_ind;
	int	array_ind;
	int	str_ind;

	if (!tok_data->line || !tok_data->in_quote_arr || !tok_data->ar
		|| tok_data->n_elems <= 0)
		return ;
	char_ind = 0;
	array_ind = 0;
	str_ind = 0;
	while (array_ind < tok_data->n_splits + 1)
	{
		tok_data->ar[array_ind] = (char *)malloc((tok_data->size_arr[array_ind]
					+ 1) * sizeof(char));
		if (tok_data->ar[array_ind] == NULL)
		{
			free_n_array(tok_data->ar, array_ind);
			return ;
		}
		while (str_ind < tok_data->size_arr[array_ind])
		{
			tok_data->ar[array_ind][str_ind] = tok_data->line[char_ind];
			char_ind++;
			str_ind++;
		}
		finish_array(tok_data->ar, &array_ind, &str_ind);
	}
	tok_data->ar[array_ind] = NULL;
}

t_tok_data	*init_tok_data(char const *line)
{
	t_tok_data	*tok_data;

	tok_data = malloc(sizeof(t_tok_data));
	if (!tok_data)
		return (NULL);
	tok_data->in_quote_arr = get_quote_state_array(line);
	if (tok_data->in_quote_arr == NULL)
	{
		free_tok_data(tok_data);
		return (NULL);
	}
	tok_data->n_elems = get_int_array_size(tok_data->in_quote_arr);
	tok_data->line = ft_substr(line, 0, tok_data->n_elems);
	tok_data->n_splits = get_n_splits(tok_data->in_quote_arr, tok_data->n_elems
			+ 1);
	tok_data->size_arr = get_sizes_arr(tok_data->in_quote_arr, tok_data->n_elems
			+ 1, tok_data->n_splits);
	if (tok_data->size_arr == NULL)
	{
		free_tok_data(tok_data);
		return (NULL);
	}
	tok_data->ar = (char **)malloc((tok_data->n_splits + 2) * sizeof(char *));
	if (tok_data->ar == NULL)
	{
		free_tok_data(tok_data);
		return (NULL);
	}
	return (tok_data);
}

void	free_tok_data(t_tok_data *tok_data)
{
	if (!tok_data)
		return ;
	free(tok_data->in_quote_arr);
	tok_data->in_quote_arr = NULL;
	free(tok_data->line);
	tok_data->line = NULL;
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
t_tok_data	*split_quotes_comments(char const *line)
{
	t_tok_data	*tok_data;

	tok_data = init_tok_data(line);
	if (!tok_data)
		return (NULL);
	printf("n_splits: %d\n", tok_data->n_splits);
	if (tok_data->n_splits == 0)
	{
		tok_data->ar[0] = ft_strdup(line);
		if (tok_data->ar[0] == NULL)
		{
			return (NULL);
		}
		tok_data->ar[1] = NULL;
	}
	else
		split_guarding_sep(tok_data);
	if (tok_data->ar[0] == NULL)
	{
		free_tok_data(tok_data);
		return (NULL);
	}
	return (tok_data);
}

/*
void	create_token_array(char **ar, t_mini *mini) {
    char **tokens;

}*/
