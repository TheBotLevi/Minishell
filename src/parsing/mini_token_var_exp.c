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

t_token* replace_expanded_tokens(t_token **head, t_token **new_tokens, t_token *start, t_token *end)
{
	t_token *tmp;
	t_token *after_end;
	t_token *next;

	after_end =NULL;
	if (end)
		after_end = end->next;
	if (start && start->prev) // Disconnect old region
		start->prev->next = *new_tokens;
	else if (!start->prev)
		*head = *new_tokens;
	token_lst_add_back(new_tokens, after_end); // Attach after_end to last of new_tokens
	// Detach old list pointers
	if (start)
		start->prev = NULL;
	if (end)
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
		free(env_val);
		free_tokens(*head);
		return (NULL);
	}
	*head = replace_expanded_tokens(head, &new_tokens, start, end);
	return (*head);
}

// returns -1 if no var exp found
static int find_next_var_exp(t_token **start, t_token **end, t_token **char_start, t_token **char_end) {
	t_token *current;

	current = *start;
	while (current){
		if (current->is_dollar) { // && !current->is_exit_status todo see if generalizes
			*start = current;
			current = current->next; //skip dollar
			if (current && current->is_braced_var)
				current = current->next; //skip brace
			*char_start = current;
			while (current && current->is_var) {
				*char_end = current;
				current = current->next;
			}
			*end = *char_end;
			if ((*char_end)->is_braced_var)
				*char_end = (*end)->prev;
			return(0);
		}
		current = current->next;
	}
	return (-1);
}

static char* lookup_var(t_parsing *parser, t_token *char_start, t_token *char_end) {
	char *str;
	char *env_val;

	if (char_start && char_start->is_exit_status) {
		env_val = ft_itoa(parser->exit_status); // todo ok to set to empty string thwn not found?
	}
	else {
		str = get_char_from_tokens(char_start, char_end);
		if (!str)
			return (NULL);
		env_val = ft_strdup(get_env_value(parser->env_struct, str));
		free(str);
		str = NULL;
	}
	if (!env_val)
		env_val = ft_strdup("");
	return (env_val);
}

/* returns 0 if a var has been expanded, -1 on error, 1 on no vars expanded
 * Do not free env_val when returned from lookup_var as it uses get_env_var which points to still in use pointer in env_struct
 */
int expand_vars(t_parsing *parser, t_token **tokens) {
	t_token *start;
	t_token *end;
	t_token *char_start;
	t_token *char_end;
	char *env_val;

	start = *tokens;
	env_val = NULL;
	if (find_next_var_exp(&start, &end, &char_start, &char_end) != 0)
		return (1);
	env_val = lookup_var(parser, char_start, char_end);
	if (!env_val)
		return (-1);
	printf("Expanding from: '%c' to '%c'\n", start->c, end->c);
	if (!start->prev && !end->next)
		printf("Full token list will be replaced\n");
	*tokens = insert_expansion_into_tokens(tokens, start, end, env_val);
	free(env_val);
	if (!*tokens)
		return (-1);
	return (0);
}
