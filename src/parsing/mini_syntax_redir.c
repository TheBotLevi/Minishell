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
//t_redirect *get_last_input_redir(t_redirect *r);
//t_redirect *get_last_output_redir(t_redirect *r);

int validate_redirections(t_redirect *redirs) {
	int seen_stdin = 0;
	int seen_stdout = 0;

	while (redirs) {
		if (redirs->type == REDIR_INPUT || redirs->type == REDIR_HEREDOC) {
			if (seen_stdin) {
				fprintf(stderr, "Ambiguous input redirection: multiple stdin sources.\n");
				return 1;
			}
			seen_stdin = 1;
		}
		if (redirs->type == REDIR_OUTPUT || redirs->type == REDIR_APPEND) {
			if (seen_stdout) {
				fprintf(stderr, "Ambiguous output redirection: multiple stdout targets.\n");
				return 1;
			}
			seen_stdout = 1;
		}
		redirs = redirs->next;
	}
	return (0);
}

//todo add test case: > cannot be the last symbol in a valid redirection in Bash.

/* Handle delimiter, can be quoted, unquoted or mix

If onlz whitespace(IFS) and then comment (e.g. cat <<    # comment),
comemnt will be seen as commernt, otherwise parsed as delim str

If any part of the delimiter is unquoted, then:
    Variable expansion, command substitution, and backslash
     interpretation will happen in the here-document body.
Bash treats the first word after << (up to the next unquoted whitespace) as the delimiter —
it can include quotes only if there's no unquoted space between parts.
 */


/*
< should redirect input
> should redirect output
>> should redirect output in append mode.
<< should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
 */

int count_redirs(t_token *start_redir, t_token *end_redir) {
	int count;

	count = 0;
	while (start_redir) {
		if (start_redir->is_redirection) {
			if (!start_redir->prev ||
				(start_redir->prev && !start_redir->prev->is_redirection))
				count++;
		}
		start_redir = start_redir->next;
		if (start_redir == end_redir)
			break;
	}
	return (count);
}

int find_next_redir( t_token **start_redir, t_token **end_redir) {
	t_token *start_redir_tmp;
	//t_token *end_redir_tmp;

	start_redir_tmp = *start_redir;
	while (start_redir_tmp && start_redir_tmp != *end_redir) {
		start_redir_tmp = start_redir_tmp->next;
	}
	//if none found: return 1
	return (1);

	return (0);
}

int get_redir_type(t_token redir_token) {
	if (redir_token.is_redir_input)
		return (REDIR_INPUT);
	if (redir_token.is_redir_output)
		return (REDIR_OUTPUT);
	if (redir_token.is_redir_heredoc)
		return (REDIR_HEREDOC);
	if (redir_token.is_redir_output_append)
		return (REDIR_APPEND);
	return (0);
}
char* get_redir_filename(t_parsing *parser, t_token *start_redir, t_token *end_redir) {
	char *filename;
	char *arg;

	arg = get_char_from_tokens(start_redir, end_redir);
	if (!arg)
		return (NULL);
	ft_strtrim(arg, parser->ifs);
	filename = ft_strdup(arg);
	free(arg);
	return (filename);
}

int parse_next_redir_tokens(t_parsing *parser, t_redirect** redir, t_token **start_redir, t_token **end_redir) {

	t_redirect *redir_tmp;
	t_token *start_redir_tmp;
	t_token *end_redir_tmp;

	redir_tmp = *redir;
	end_redir_tmp = *end_redir;
	find_next_redir(&start_redir_tmp, &end_redir_tmp);
	redir_tmp->filename = get_redir_filename(parser, start_redir_tmp, end_redir_tmp);
	if (!redir_tmp->filename)
		return (1);
	redir_tmp->type = get_redir_type(*start_redir_tmp);
	*redir = redir_tmp;
	*start_redir = end_redir_tmp;// set new starting point to old end point
	return(0);
}

int create_redirs(t_parsing *parser, t_token *start_redir, t_token *end_redir) {

	t_command **cmd;
	int count_redir;
	t_redirect *head_redir;
	t_redirect *redir;
	t_redirect *prev;
	/*t_redirect *last_redir;
	t_token *current;*/
	int i;
	//validate_redirections()

	cmd = &(parser->current_cmd);
	head_redir = NULL;
	i = 0;
	prev = NULL;
	count_redir = count_redirs(start_redir, end_redir);
	while (i < count_redir) {
		redir = malloc(sizeof(t_redirect));
		memset(redir, 0, sizeof(t_redirect));
		if (i == 0) {
			head_redir = redir;
			(*cmd)->redirections = head_redir;
		}
		else
			prev->next = redir;
		if (parse_next_redir_tokens(parser, &redir, &start_redir, &end_redir))
			return(1);
		if (!redir || !end_redir)
			break;
		if (end_redir->is_redirection) //todo check  if reset to start, then check start instead
			end_redir = end_redir->next;
		prev = redir;
		i++;
	}
	return (0);
}

// HEREDOC complexity todo look for quoted argument
//next->is_comment = 0; //todo deal with this in syntax parsing
//next->is_comment_start = 0;


/*update end_token to point to after redirection, update tokens and remove the redirection tokens*/
int detect_redir(t_parsing *parser, t_token **cur_token, t_token **start_redir, t_token **end_cmd) {
	t_command **cmd;
	t_token *current;
	int has_redir;

	cmd = &(parser->current_cmd);
	has_redir = 0;
	current = *cur_token;
	while (current && !current->is_pipe) {
		if (current->is_redirection && !has_redir) {
			has_redir = 1;
			*start_redir = current;
			if (current->is_redir_heredoc) // todo correct to only take into account if first/only redirection?
				(*cmd)->is_heredoc = 1;
			//when comment add stop? except heredoc
		if (!(*cmd)->is_heredoc && current->is_comment_start){
			//todo do sth -> stop early and end cmd?
			//*end_cmd = current;
			// if (has_redir)
			//		*cur_token = create_redirs(&cmd, start_redir, end_cmd);
			// return(1);
			break;
			}
		}
		current = current->next;
	}
	*end_cmd = current;
	if (has_redir) {
		if (create_redirs(parser, *start_redir, *end_cmd))
			return (1);
	}
	*end_cmd = current;
	return (0);
}
