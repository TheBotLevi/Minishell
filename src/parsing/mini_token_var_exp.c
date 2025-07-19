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

static t_token* replace_expanded_tokens(t_token **head, t_token **new_tokens, t_token *start, t_token *end)
{
	t_token *tmp;

	if (!end->next && start->prev) { // starts in the middle and ends on last elem of the list
		start->prev->next = *new_tokens;
		while (start){ //free from start to end
			tmp = start;
			start = start->next;
			free(tmp);
		}
	}
	else if (end->next && !start->prev && start == *head) //start is head and end is in middle
	{
		while (*head && *head != end){ //free from head to end-1
			tmp = *head;
			head = &(*head)->next;
			free(tmp);
		}
		token_lst_add_back(new_tokens, end->next); // last new token->next becomes end->next
		*head = *new_tokens;
		free(end);
	}
	else if (end->next && start->prev) //starts in the middle and end is in middle
	{
		token_lst_add_back(new_tokens, end->next); // append old tokens to new list
		start->prev->next = *new_tokens; // point from start-1 to new list
		while (start && start != end){ //free from start to end
			tmp = start;
			start = start->next;
			free(tmp);
		}
		free(end);
	}
	return (*head);
}

static t_token *insert_expansion_into_tokens(t_token **head, t_token *start, t_token *end, char *env_val) {
	t_token **new_tokens;

	if (!start || !end || !env_val) {
		free_tokens(head);
		return (NULL);
	}
	new_tokens = (t_token **)malloc(sizeof(t_token *));
	if (!new_tokens){
		free_tokens(head);
		return (NULL);
	}
	*new_tokens = NULL;
	if (create_basic_tokens(env_val, new_tokens)) {
		free(env_val);
		free_tokens(head);
		free(new_tokens);
		return (NULL);
	}
	*head = replace_expanded_tokens(head, new_tokens, start, end);
	free(new_tokens);
	return (*head);
}

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
		env_val = get_env_value(parser->env_struct, str);
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
	*tokens = insert_expansion_into_tokens(tokens, start, end, env_val);
	free(env_val);
	if (!*tokens)
		return (-1);
	return (0);
}
