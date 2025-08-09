/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_token_var_exp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:07:50 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/18 18:07:53 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// caller needs to ensure that start and end are not null!
t_token* replace_expanded_tokens(t_token **head, t_token **new_tokens, t_token *start, t_token *end)
{
	t_token *tmp;
	t_token *after_end;
	t_token *next;

	after_end = end->next;
	if (*new_tokens) {
		if (start->prev) {
			start->prev->next = *new_tokens;
			(*new_tokens)->prev = start->prev;
		} else {
			*head = *new_tokens;
			(*new_tokens)->prev = NULL;
		}
		token_lst_add_back(new_tokens, after_end);
	} else {
		// new_tokens is NULL — connect start->prev to after_end
		if (start->prev)
			start->prev->next = after_end;
		else
			*head = after_end;
		if (after_end)
			after_end->prev = start->prev;
	}
	// Detach old list pointers
	start->prev = NULL;
	end->next = NULL;
	tmp = start;
	while (tmp) { // Free only from start to end (inclusive)
		next = tmp->next;
		free(tmp);
		if (tmp == end)
			break;
		tmp = next;
	}
	return (*head);
}

static t_token *insert_expansion_into_tokens(t_token **head, t_token *start, t_token *end, char *env_val) {
	t_token *new_tokens;

	if (!start || !end || !env_val) {
		free_tokens(*head);
		return (NULL);
	}
	new_tokens = NULL;
	if (create_basic_tokens(env_val, &new_tokens)) {
		free_tokens(*head);
		return (NULL);
	}
	*head = replace_expanded_tokens(head, &new_tokens, start, end);
	return (*head);
}

// returns 1 if no var exp found
static int find_next_var_exp(t_token **start, t_token **end, t_token **char_start, t_token **char_end) {
	t_token *current;

	current = *start;
	while (current){
		if (current->is_dollar) {
			*start = current;
			current = current->next;
			*char_start = current;
			while (current && current->is_var) {
				*char_end = current;
				current = current->next;
			}
			*end = *char_end;
			return(0);
		}
		current = current->next;
	}
	return (1);
}

static char* lookup_var(t_parsing *parser, t_token *char_start, t_token *char_end) {
	char *str;
	char *env_val;

	env_val = NULL;
	if (char_start && char_start->is_exit_status) {
		env_val = ft_itoa(parser->exit_status);
	}
	else {
		str = get_char_from_tokens(char_start, char_end);
		if (!str)
			return (NULL);
		env_val = get_env_value(parser->env_struct, str);
		if (env_val)
			env_val = ft_strdup(get_env_value(parser->env_struct, str));
		free(str);
		str = NULL;
	}
	if (!env_val)
		env_val = ft_strdup("");
	return (env_val);
}

/* returns 0 if no expansions or if all vars has been successfully expanded, -1 on error
 */
int expand_vars(t_parsing *parser, t_token **tokens)
{
	t_token *start;
	t_token *end;
	t_token *char_start;
	t_token *char_end;
	char *env_val;

	set_var_expansion_flags(tokens);
	start = *tokens;
	env_val = NULL;
	char_end = NULL;
	char_start = NULL;
	while (find_next_var_exp(&start, &end, &char_start, &char_end) == 0){
		env_val = lookup_var(parser, char_start, char_end);
		if (!env_val)
			return (1);
		printf("Expanding from: '%c' to '%c'\n", start->c, end->c);
		if (!start->prev && !end->next)
			printf("Full token list will be replaced\n");
		*tokens = insert_expansion_into_tokens(tokens, start, end, env_val);
		free(env_val);
		env_val = NULL;
		if (!*tokens)
			return (1);
		start = *tokens;
	}
	return (0);
}
