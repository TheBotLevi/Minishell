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

int count_redirs(t_token *start_redir, const t_token *end_cmd) {
	int count;

	count = 0;
	while (start_redir) {
		if (start_redir->is_redirection) {
			if (!start_redir->prev ||
				(start_redir->prev && !start_redir->prev->is_redirection))
				count++;
		}
		start_redir = start_redir->next;
		if (start_redir == end_cmd)
			break;
	}
	return (count);
}

// inclusive of next redir token
static int find_next_redir( t_token **start_redir, t_token **end_redir, const t_token *end_cmd) {
	t_token *start_redir_tmp;
	int start_set;

	start_set = 0;
	if (!*start_redir)
		return (1);
	start_redir_tmp = *start_redir;
	while (start_redir_tmp && start_redir_tmp != end_cmd) {
		if (start_redir_tmp->is_redirection && !start_set) {
			*start_redir = start_redir_tmp;
			start_set = 1;
			if (start_redir_tmp->is_redir_heredoc || start_redir_tmp->is_redir_output_append) //skip double redir signage
				*start_redir = start_redir_tmp->next;
		}
		else if (start_redir_tmp->is_redirection && start_set) {
			*end_redir = start_redir_tmp;
			return (0);
		}
		start_redir_tmp = start_redir_tmp->next;
	}
	if (!start_redir_tmp || ( start_redir_tmp == end_cmd)) {
		// in case redir goes till EO Command
		*end_redir = (t_token *)end_cmd;
		return (0);
	}
	return (1);
}

int get_redir_type(t_token *redir_token) {
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
/* todo see if skip or remove
t_token* elim_redir_tokens(t_token **head, t_token **new_tokens, t_token *start, t_token *end)
{
	t_token *tmp;
	t_token *after_end;
	t_token *next;

	after_end =NULL;
	if (end)
		after_end = end->next;
	if (start && start->prev) // Disconnect old region
		start->prev->next = *new_tokens;
	else if (!start->prev)
		*head = *new_tokens;
	token_lst_add_back(new_tokens, after_end); // Attach after_end to last of new_tokens
	// Detach old list pointers
	if (start)
		start->prev = NULL;
	if (end)
		end->next = NULL;
	tmp = start;
	while (tmp) { // Free only from start to end (inclusive)
		next = tmp->next;
		free(tmp);
		if (tmp == end)
			break;
		tmp = next;
	}
	return (*head);
}*/

int get_array_size(char **array)
{
	int size;

	size = 0;
	while (array[size])
		size++;
	return (size);
}

char* get_redir_filename(t_parsing *parser, t_token *start_redir, t_token *end_redir) {
	char *filename;
	//char *arg;
	(void)parser;
	char **args;

	if (!start_redir) // if last element with !end_redir do anything specific?
		return (NULL);
	if (start_redir->is_redirection) { //skip redirection symbols --> alreadz handled in get next redir
		if (start_redir->is_redir_heredoc || start_redir->is_redir_output_append)
			start_redir = start_redir->next;
		start_redir = start_redir->next;
	}
	if (end_redir && (end_redir->is_redirection || end_redir->is_pipe)) { //skip back before redirection symbols if included
		end_redir = end_redir->prev;
	}/*
	else if (!end_redir) { //set to last element
		end_redir = start_redir;
		while (end_redir && end_redir->next)
			end_redir = end_redir->next;
	}*/
	/*arg = get_char_from_tokens(start_redir, end_redir);
	if (!arg)
		return (NULL);
	ft_strtrim(arg, parser->ifs);
	free(arg);
	*/
	args = ft_split_on_ifs(start_redir, end_redir);
	if (!args) {
		printf("Syntax error: No redirection arguments: what should happen?\n"); //todo should fail here?
		return (NULL);
	}
	if (get_array_size(args) > 1) {
		printf("Multiple redirection arguments: what should happen?\n"); //todo should fail here?
		free_args(args);
		return (NULL);
	}
	filename = ft_strdup(args[0]);
	free_args(args);
	return (filename);
}

int parse_next_redir_tokens(t_parsing *parser, t_redirect* redir, t_token **start_redir, const t_token *end_cmd) {

	t_token *start_redir_tmp;
	t_token *end_redir_tmp;

	start_redir_tmp = *start_redir;
	end_redir_tmp = NULL;
	redir->type = get_redir_type(start_redir_tmp);
	if (find_next_redir(&start_redir_tmp, &end_redir_tmp, end_cmd))
		return (1);
	redir->filename = get_redir_filename(parser, start_redir_tmp, end_redir_tmp);
	if (!redir->filename)
		return (1);
	/*if (end_redir_tmp && end_redir_tmp->is_redirection) { // skip necessary redir symbols
		if ((end_redir_tmp->is_redir_heredoc || end_redir_tmp->is_redir_output_append) &&
			end_redir_tmp->next)
			end_redir_tmp = end_redir_tmp->next;

		if (end_redir_tmp && end_redir_tmp->next)
			end_redir_tmp = end_redir_tmp->next;
	}*/
	*start_redir = end_redir_tmp;// set new starting point to old end point
	return (0);
}

int create_redirs(t_parsing *parser, t_token *start_redir, const t_token *end_cmd) {

	int count_redir;
	t_redirect *redir;
	t_redirect *prev;
	/*t_redirect *last_redir;
	t_token *current;*/
	int i;
	//validate_redirections() todo

	i = 0;
	prev = NULL;
	count_redir = count_redirs(start_redir, end_cmd);
	while ((i < count_redir) && (start_redir != end_cmd)) { //if (start_redir == end_redirs): all redirections parsed
		redir = malloc(sizeof(t_redirect));
		if (!redir)
			return (1);
		memset(redir, 0, sizeof(t_redirect));
		if (i == 0)
			parser->current_cmd->redirections = redir;
		else
			prev->next = redir;
		if (parse_next_redir_tokens(parser, redir, &start_redir, end_cmd))
			return (1);
		prev = redir;
		if (!start_redir)
			break;

		i++;
	}
	return (0);
}

/*update end_token to point to after redirection, update tokens and remove the redirection tokens*/
int detect_redir(t_parsing *parser, t_token *cmd_start, t_token **start_redir, const t_token *end_cmd) {
	t_token *current;
	int has_redir;

	has_redir = 0;
	current = cmd_start;
	while (current && current != end_cmd) {
		if (current->is_redirection && !has_redir) {
			has_redir = 1;
			*start_redir = current;
			if (current->is_redir_heredoc) // todo correct to only take into account if first/only redirection?
				parser->current_cmd->is_heredoc = 1;
			//when comment add stop? except heredoc
		if (!parser->current_cmd->is_heredoc && current->is_comment_start){
			//todo do sth -> stop early and end cmd?
			//*end_cmd = current;
			// if (has_redir)
			//		*cmd_start = create_redirs(&cmd, start_redir, end_cmd);
			// return(1);
			break;
			}
		}
		current = current->next;
	}
	return (has_redir);
}

// HEREDOC complexity todo look for quoted argument
//next->is_comment = 0; //todo deal with this in syntax parsing
//next->is_comment_start = 0;
