/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:22:08 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/03 12:41:17 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	mark_exit_status(t_token **current, t_token **next)
{
	(*current)->is_exit_status = 1;
	(*next)->is_exit_status = 1;
	(*next)->is_var = 1;
}

void	mark_braced_var(t_token **current, t_token *next)
{
	t_token	*start;

	start = next->prev;
	if (next && next->c == '{')
	{
		next = next->next;
		if (next && (ft_isalpha(next->c) || next->c == '_'))
		{
			next->is_var = 1;
			next = next->next;
			while (next && next->c != '}' && (ft_isalnum(next->c)
					|| next->c == '_'))
				next = next->next;
			if (next && next->c == '}')
			{
				*current = next;
				while (next && next != start)
				{
					next->is_braced_var = 1;
					next->is_var = 1;
					next = next->prev;
				}
			}
		}
	}
}

void	mark_unbraced_var(t_token **current, t_token **next)
{
	(*next)->is_var = 1;
	*next = (*next)->next;
	while (*next && (ft_isalnum((*next)->c) || (*next)->c == '_'))
	{
		(*next)->is_var = 1;
		*next = (*next)->next;
	}
	*current = *next; // skip to after var name
}

void	set_var_expansion_flags(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current && !current->is_comment)
	{
		next = current->next;
		if (current->c == '$' && !current->is_single_quote)
		{
			current->is_dollar = 1;
			if (next && next->c == '?')
				mark_exit_status(&current, &next);
			else if (next && (ft_isalpha(next->c) || next->c == '_'))
				mark_unbraced_var(&current, &next);
			else
				mark_braced_var(&current, next);
		}
		if (current)
			current = current->next;
	}
}

t_token	**tokenize(char *line, t_mini *mini)
{
	t_token	**tokens;

	(void)mini;
	if (!line || !mini)
		return (NULL);
	tokens = (t_token **)malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	*tokens = NULL;
	if (create_basic_tokens(line, tokens, mini) == 0)
	{
		set_quote_flags(tokens);
		mark_comment(tokens);
		set_pipe_flags(tokens);
		set_redirection_flags(tokens);
		set_is_redirection_flag(tokens);
		set_var_expansion_flags(tokens);
		set_ifs_flags(mini, tokens);
		print_tokens(*tokens); // todo delete DEBUG
	}
	return (tokens);
}
