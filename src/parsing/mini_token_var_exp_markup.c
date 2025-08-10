/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_token_var_exp_markup.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 08:33:40 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/10 08:33:42 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	mark_exit_status(t_token **current, t_token **next)
{
	(*current)->is_exit_status = 1;
	(*next)->is_exit_status = 1;
	(*next)->is_var = 1;
}

void	mark_unbraced_var(t_token **current, t_token **next)
{
	t_token	*last_var_char;

	last_var_char = *next;
	(*next)->is_var = 1;
	*next = (*next)->next;
	while (*next && (ft_isalnum((*next)->c) || (*next)->c == '_'))
	{
		(*next)->is_var = 1;
		last_var_char = *next;
		*next = (*next)->next;
	}
	*current = last_var_char;
}

void	set_var_expansion_flags(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current && !current->is_redir_heredoc_delimiter)
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
				current->is_dollar = 0;
		}
		if (current)
			current = current->next;
	}
}
