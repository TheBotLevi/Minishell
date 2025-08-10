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

/*
 *
*✅ Typical Redirection Conflicts to Detect
1. Multiple input redirections (<, <<)

	cat < input1 < input2 → ambiguous: which one should apply?

	cat < input << EOF → also ambiguous: file vs. heredoc

2. Multiple output redirections (>, >>)

	ls > out1 > out2 → only out2 will be used

3. Mix of > and >>

	echo hi > file >> file → strange but technically valid, last wins
 *
 */

// silently allow multiple redirections but only use the last one for each stream:
// t_redirect *get_last_input_redir(t_redirect *r);
// t_redirect *get_last_output_redir(t_redirect *r);
/*
int	validate_redirections(t_redirect *redirs) {
	int seen_stdin = 0;
	int seen_stdout = 0;

	while (redirs) {
		if (redirs->type == REDIR_INPUT || redirs->type == REDIR_HEREDOC) {
			if (seen_stdin) {
				fprintf(stderr,
					"Ambiguous input redirection: multiple stdin sources.\n");
				return (1);
			}
			seen_stdin = 1;
		}
		if (redirs->type == REDIR_OUTPUT || redirs->type == REDIR_APPEND) {
			if (seen_stdout) {
				fprintf(stderr,
					"Ambiguous output redirection: multiple stdout targets.\n");
				return (1);
			}
			seen_stdout = 1;
		}
		redirs = redirs->next;
	}
	return (0);
}*/

int	get_redir_type(const t_token *redir_token)
{
	if (redir_token->is_redir_input)
		return (REDIR_INPUT);
	if (redir_token->is_redir_output)
		return (REDIR_OUTPUT);
	if (redir_token->is_redir_heredoc)
		return (REDIR_HEREDOC);
	if (redir_token->is_redir_output_append)
		return (REDIR_APPEND);
	return (0);
}

void	find_str_filename_tokens(t_token **start_redir, t_token **end_redir)
{
	t_token	*cur;
	t_token *last_filename;

	cur = *start_redir;
	last_filename = NULL;

	// Find the start of the filename
	while (cur && cur != *end_redir && !cur->is_redir_filename)
		cur = cur->next;
	if (!cur || cur == *end_redir)
	{
		*start_redir = NULL;
		*end_redir = NULL;
		return;
	}

	*start_redir = cur;

	// Track the last filename token
	while (cur && cur != *end_redir && cur->is_redir_filename)
	{
		last_filename = cur;
		cur = cur->next;
	}

	// Make the end inclusive
	*end_redir = last_filename;
}
/*
void	find_str_filename_tokens(t_token **start_redir, t_token **end_redir)
{
	t_token	*cur;

	cur = *start_redir;
	while (cur && cur != *end_redir && !cur->is_redir_filename)
		cur = cur->next;
	if (!cur || cur == *end_redir)
	{
		*start_redir = NULL;
		*end_redir = NULL;
		return;
	}
	*start_redir = cur;
	while (cur && cur != *end_redir && cur->is_redir_filename)
	{
		*end_redir = cur;
		cur = cur->next;
	}

}*/

int	check_for_fully_quoted_str_redir(const t_token *start_redir,
		const t_token *end_redir)
{
	t_token	*cur;

	cur = (t_token*) start_redir;
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
char	*get_redir_filename(t_token *start_redir, t_token *end_redir,
	int *is_fully_quoted, t_token **last_redir_token)
{
	char	*filename;
	char	**args;

	if (!start_redir || !start_redir->is_redirection)
		return (NULL);
	find_str_filename_tokens(&start_redir, &end_redir);
	if (!start_redir || !end_redir)
		return (NULL);
	*last_redir_token = end_redir;
	*is_fully_quoted = check_for_fully_quoted_str_redir(start_redir, end_redir);
	args = ft_split_on_ifs(start_redir, end_redir->next, 0);
	if (!args)
	{
		if (end_redir && end_redir->next)
			print_unexpected_token_error(end_redir->next);
		else
			print_unexpected_token_error(end_redir);
		return (NULL);
	}
	filename = ft_strdup(args[0]);
	free_args(args);
	return (filename);
}

void	redirection_lst_add_back(t_redirect **redir_head, t_redirect *new_redir)
{
	t_redirect	*last_elem;

	if (!redir_head || !new_redir)
		return ;
	if (*redir_head == NULL)
	{
		*redir_head = new_redir;
		return ;
	}
	last_elem = *redir_head;
	while (last_elem->next)
		last_elem = last_elem->next;
	last_elem->next = new_redir;
}

// returns null on allocation error
int	create_redirection(const t_parsing *parser, const t_token *start_redir,
		const t_token *end_cmd, t_token **last_redir_token)
{
	t_redirect	*redir;

	if (end_cmd)
		printf("creating redirection from: token no %d ('%c') to token "
				"no % d('%c')\n ",
				start_redir->idx,
				start_redir->c,
				end_cmd->idx,
				end_cmd->c);
	else
		printf("creating redirection from: token no %d ('%c') till EOCommand\n",
			start_redir->idx, start_redir->c);
	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (1);
	memset(redir, 0, sizeof(t_redirect));
	redir->type = get_redir_type(start_redir);
	if (redir->type == REDIR_HEREDOC)
		parser->current_cmd->has_heredoc = 1;
	redir->filename = get_redir_filename((t_token*)start_redir,
		(t_token*) end_cmd, &(redir->is_quoted), last_redir_token);
	if (!redir->filename)
		return (1);
	if (*last_redir_token)
		printf("last redirect token is %c no %d\n", (*last_redir_token)->c,(*last_redir_token)->idx);
	redirection_lst_add_back(&(parser->current_cmd->redirections), redir);
	return (0);
}

// returns start_first_redir: NULL if no redirection found
int parse_redirections(t_parsing *parser, const t_token *start_cmd,
		const t_token *end_cmd)
{
	//t_token			*start_first_redir;
	t_token			*end_redir;
	/*const t_token	*redir_start;
	int				start_set;

	start_first_redir = NULL;
	redir_start = NULL;
	start_set = 0;*/
	while (start_cmd && start_cmd != end_cmd) {
		if (start_cmd->is_redirection) {
			if (create_redirection(parser, start_cmd, end_cmd, &end_redir))
				return (1);
			start_cmd = end_redir->next;
		}
		if (start_cmd)
			start_cmd = start_cmd->next;
	}
		/*
		if (start_cmd->is_redirection)
		{
			if (!start_first_redir)
				start_first_redir = (t_token *)start_cmd;
			if (!start_set)
			{
				start_set = 1;
				redir_start = start_cmd;
				if (start_cmd->is_redir_heredoc
					|| start_cmd->is_redir_output_append)
					start_cmd = start_cmd->next;
			}
			else
			{
				if (create_redirection(parser, redir_start,start_cmd))
					return (1);
				start_set = 0;
				continue ;
			}
		}
		start_cmd = start_cmd->next;
	}
	if (start_set)
		if (create_redirection(parser, redir_start, (t_token *)end_cmd))
			return (1);*/
	return (0);
}
