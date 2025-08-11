/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_syntax_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 13:57:20 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/19 13:57:26 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	set_redir_type(t_parsing *parser, t_redirect *redir,
		const t_token *redir_token)
{
	if (redir_token->is_redir_input)
		redir->type = REDIR_INPUT;
	else if (redir_token->is_redir_output)
		redir->type = REDIR_OUTPUT;
	else if (redir_token->is_redir_heredoc)
	{
		redir->type = REDIR_HEREDOC;
		parser->current_cmd->has_heredoc = 1;
	}
	else if (redir_token->is_redir_output_append)
		redir->type = REDIR_APPEND;
	else
		return (1);
	return (0);
}

static void	find_str_filename_tokens(t_token **start_redir, t_token **end_redir)
{
	t_token	*cur;

	cur = *start_redir;
	while (cur && cur != *end_redir && !cur->is_redir_filename)
		cur = cur->next;
	if (!cur || cur == *end_redir)
	{
		*start_redir = NULL;
		*end_redir = NULL;
		return ;
	}
	*start_redir = cur;
	while (cur && cur != *end_redir && cur->is_redir_filename)
	{
		*end_redir = cur;
		cur = cur->next;
	}
}

static int	check_for_fully_quoted_str_redir(const t_token *start_redir,
		const t_token *end_redir)
{
	t_token	*cur;

	cur = (t_token *)start_redir;
	if (!cur || cur == end_redir)
		return (0);
	if (!(cur->is_start_quote && end_redir->is_end_quote))
		return (0);
	while (cur)
	{
		if (!cur->is_quote || (cur != end_redir && cur->is_end_quote))
			return (0);
		if (cur == end_redir)
			return (1);
		cur = cur->next;
	}
	return (0);
}

// receives start_redir token that is a redirection and
// end redir as NULL or the next operator
static int	get_redir_filename(t_token *start_redir, t_token *end_redir,
		t_redirect *redir, t_token **last_redir_token)
{
	char	**args;

	if (!start_redir || !start_redir->is_redirection)
		return (1);
	find_str_filename_tokens(&start_redir, &end_redir);
	if (!start_redir || !end_redir)
		return (1);
	*last_redir_token = end_redir;
	redir->is_quoted = check_for_fully_quoted_str_redir(start_redir, end_redir);
	args = ft_split_on_ifs(start_redir, end_redir->next, 0);
	if (!args)
	{
		if (end_redir && end_redir->next)
			print_unexpected_token_error(end_redir->next);
		else
			print_unexpected_token_error(end_redir);
		return (1);
	}
	redir->filename = ft_strdup(args[0]);
	free_args(args);
	return (0);
}

// returns start_first_redir: NULL if no redirection found
int	parse_redirections(t_parsing *parser, t_token *start_cmd, t_token *end_cmd)
{
	t_token		*end_redir;
	t_redirect	*redir;

	while (start_cmd && start_cmd != end_cmd)
	{
		if (start_cmd->is_redirection)
		{
			redir = malloc(sizeof(t_redirect));
			if (!redir)
				return (2);
			memset(redir, 0, sizeof(t_redirect));
			if (set_redir_type(parser, redir, start_cmd)
				|| get_redir_filename(start_cmd, end_cmd, redir, &end_redir))
			{
				free(redir);
				return (3);
			}
			redir_lst_add_back(&(parser->current_cmd->redirections), redir);
			start_cmd = end_redir->next;
		}
		if (start_cmd)
			start_cmd = start_cmd->next;
	}
	return (0);
}
