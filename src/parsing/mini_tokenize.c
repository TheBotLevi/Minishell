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
					if (next == start) {
						next->is_braced_var = 1;
						next->is_var = 1;
					}
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

void	unset_all_flags(t_token *current)
{
	t_token	*next;
	t_token	*prev;
	char c;

	while (current) {
		prev = current->prev;
		next = current->next;
		c = current->c;
		ft_memset(current, 0, sizeof(t_token));
		current->prev = prev;
		current->next = next;
		current->c = c;
		current = next;
	}
}

void reset_idx(t_token *tokens) {
	t_token	*current;
	int	i;

	i = 0;
	current = tokens;
	while (current) {
		current->idx = i;
		current = current->next;
		i++;
	}
}

void	set_heredoc_delimiter_flags(t_parsing *parser, t_token *tokens)
{
	t_token *current;

	current = tokens;
	while (current)
	{
		if (!current->is_quote && current->c == '<' && current->next && current->next->c == '<')
		{
			current = current->next->next; // skip over '<<'
			while (current && is_in_set(current->c, parser->ifs)) // Skip whitespace
				current = current->next;
			while (current && !is_in_set(current->c,parser->ifs) && !is_in_set(current->c,"|><")) // Mark the next word (the delimiter)
			{
				current->is_redir_heredoc_delimiter = 1;
				current = current->next;
			}
		}
		else
			current = current->next;
	}
}

t_token	*tokenize(char *line, t_parsing *parser)
{
	t_token	*tokens;
	int n_pipes;

	if (!line || !parser)
		return (NULL);
	tokens = NULL;
	n_pipes = 0;
	if (create_basic_tokens(line, &tokens) == 0) {
		if (set_quote_flags(tokens)){
			ft_putendl_fd("syntax error: unclosed quote", STDERR_FILENO);
			free_tokens(tokens);
			return (NULL);
		}
		set_heredoc_delimiter_flags(parser, tokens);
		mark_comment(tokens);
		set_var_expansion_flags(&tokens);
		while (expand_vars(parser, &tokens) == 0) {
			unset_all_flags(tokens);
			if (set_quote_flags(tokens)){
				ft_putendl_fd("syntax error: unclosed quote", STDERR_FILENO);
				free_tokens(tokens);
				return (NULL);
			}
			set_heredoc_delimiter_flags(parser, tokens);
			mark_comment(tokens);
			set_var_expansion_flags(&tokens);
		}
		reset_idx(tokens);
		n_pipes = set_pipe_flags(&tokens);
		set_redirection_flags(tokens);
		flag_is_redirection(tokens);
		set_ifs_flags(parser, &tokens);
		/*printf("all var exp finished:\n");  // todo delete DEBUG
		print_tokens(*tokens);*/
	}
	parser->n_cmds = n_pipes + 1;
	return (tokens);
}
