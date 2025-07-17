/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_token_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:29:32 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/11 16:29:35 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	free(tokens);
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("%c: ifs:%d, quote:%d, \':%d, \":%d, start\":%d, end\":%d,"
				"#_start:%d, #:%d, |:%d, $:%d, $?:%d, $var:%d, ${}:%d, <:%d,"
				"<<:%d, <<_del: %d, >:%d, >>:%d \n",
				current->c,
				current->is_ifs,
				current->is_quote,
				current->is_single_quote,
				current->is_double_quote,
				current->is_start_quote,
				current->is_end_quote,
				current->is_comment_start,
				current->is_comment,
				current->is_pipe,
				current->is_dollar,
				current->is_exit_status,
				current->is_var,
				current->is_braced_var,
				current->is_redir_input,
				current->is_redir_heredoc,
				current->is_redir_heredoc_delimiter,
				current->is_redir_output,
				current->is_redir_output_append);
		current = current->next;
	}
}

int	create_basic_tokens(char *line, t_token **tokens)
{
	int		i;
	t_token	*token;
	t_token	*prev;

	i = 0;
	prev = NULL;
	while (line[i])
	{
		token = (t_token *)malloc(sizeof(t_token));
		if (!token)
		{
			free_tokens(tokens);
			return (1);
		}
		ft_memset(token, 0, sizeof(t_token));
		token->c = line[i];
		token->next = NULL;
		token->prev = prev;
		if (prev)
			prev->next = token;
		prev = token;
		if (i == 0)
			*tokens = prev;
		i++;
	}
	return (0);
}
