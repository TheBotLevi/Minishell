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

void	reset_flags(t_token *current)
{
	t_token	*next;
	t_token	*prev;
	char	c;
	int		i;

	i = 0;
	while (current)
	{
		prev = current->prev;
		next = current->next;
		c = current->c;
		ft_memset(current, 0, sizeof(t_token));
		current->prev = prev;
		current->next = next;
		current->c = c;
		current->idx = i++;
		current = next;
	}
}

int	set_quotes_and_heredoc(t_parsing *parser, t_token *tokens)
{
	if (mark_quote_flags(tokens))
	{
		ft_putendl_fd("mariashell: syntax error: unclosed quote",
			STDERR_FILENO);
		free_tokens(tokens);
		return (1);
	}
	if (set_heredoc_delimiter_flags(parser, tokens))
		return (1);
	return (0);
}

static t_token	*mark_heredoc_delim(t_token *current, int *empty_delim,
		t_parsing *parser)
{
	current = current->next->next;
	while (current && is_in_set(current->c, parser->ifs))
		current = current->next;
	while (current && !is_in_set(current->c, parser->ifs)
		&& !is_in_set(current->c, "|><"))
	{
		*empty_delim = 0;
		current->is_redir_heredoc_delimiter = 1;
		current = current->next;
	}
	return (current);
}

// returns 1 for syntax error if heredoc is present but delimiter is empty
int	set_heredoc_delimiter_flags(t_parsing *parser, t_token *tokens)
{
	t_token	*current;
	int		empty_delim;

	current = tokens;
	empty_delim = 0;
	while (current)
	{
		if (!current->is_quote && current->c == '<' && current->next
			&& current->next->c == '<')
		{
			empty_delim = 1;
			current = mark_heredoc_delim(current, &empty_delim, parser);
			if (empty_delim)
			{
				print_unexpected_token_error(current);
				free_tokens(tokens);
				return (1);
			}
		}
		else
			current = current->next;
	}
	return (empty_delim);
}

t_token	*tokenize(char *line, t_parsing *parser)
{
	t_token	*tokens;

	if (!line || !parser)
		return (NULL);
	tokens = NULL;
	if (create_basic_tokens(line, &tokens))
		return (NULL);
	if (set_quotes_and_heredoc(parser, tokens))
		return (NULL);
	if (expand_vars(parser->exit_status, parser->env_struct, &tokens))
	{
		ft_putendl_fd("mariashell: memory allocation error during "
			"variable expansion",
			STDERR_FILENO);
		free_tokens(tokens);
		return (NULL);
	}
	reset_flags(tokens);
	if (set_quotes_and_heredoc(parser, tokens))
		return (NULL);
	set_ifs_flags(parser, &tokens);
	if (set_pipe_flags(parser, &tokens))
		return (NULL);
	if (set_redirection_flags(&tokens))
		return (NULL);
	return (tokens);
}
