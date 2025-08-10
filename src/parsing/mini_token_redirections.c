/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_token_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 09:31:01 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/10 09:31:03 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// marks and advances pointer over 2nd redirection token
void	set_double_redir_flags(t_token **current)
{
	if ((*current)->c == '>')
	{
		(*current)->is_redir_output_append = 1;
		(*current)->next->is_redir_output_append = 1;
	}
	else if ((*current)->c == '<')
	{
		(*current)->is_redir_heredoc = 1;
		(*current)->next->is_redir_heredoc = 1;
	}
	(*current) = (*current)->next;
}

static t_token	*advance_to_redir_content(t_token *current)
{
	current = current->next;
	if (current && (current->is_redir_heredoc
			|| current->is_redir_output_append))
		current = current->next;
	while (current && current->is_ifs)
		current = current->next;
	return (current);
}

static t_token	*mark_redir_filename(t_token *current, int *empty_filename)
{
	while (current && !current->is_ifs && (!current->is_redirection
			&& !current->is_pipe))
	{
		*empty_filename = 0;
		current->is_redir_filename = 1;
		current = current->next;
	}
	return (current);
}

// returns 1 for syntax error if heredoc is present but delimiter is empty
int	set_redir_filename_flags(t_token *tokens)
{
	t_token	*current;
	int		empty_filename;

	current = tokens;
	empty_filename = 0;
	while (current)
	{
		if (current->is_redirection)
		{
			empty_filename = 1;
			current = advance_to_redir_content(current);
			current = mark_redir_filename(current, &empty_filename);
			if (empty_filename)
			{
				print_unexpected_token_error(current);
				free_tokens(tokens);
				return (1);
			}
		}
		else
			current = current->next;
	}
	return (empty_filename);
}

// marks list of tokens with redirection flags
int	set_redirection_flags(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (!current->is_quote && !current->is_redir_heredoc_delimiter)
		{
			if (current->next && ((current->c == '>' && current->next->c == '>')
					|| (current->c == '<' && current->next->c == '<')))
				set_double_redir_flags(&current);
			else if (current->c == '>')
				current->is_redir_output = 1;
			else if (current->c == '<')
				current->is_redir_input = 1;
		}
		current = current->next;
	}
	flag_is_redirection(*tokens);
	if (set_redir_filename_flags(*tokens))
		return (1);
	return (0);
}
