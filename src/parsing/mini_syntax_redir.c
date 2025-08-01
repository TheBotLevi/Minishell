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
Variable expansion, command substitution, and backslash interpretation will happen in the here-document body.
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


int get_redir_type(const t_token *redir_token) {
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

/*doesnt check last element since impossible to have just one quote token within a redirection*/
int contains_quote(const t_token *start_redir, const t_token *end_redir) {

	while (start_redir && start_redir != end_redir) {
		if (start_redir->is_quote)
			return (1);
		start_redir = start_redir->next;
	}
	return (0);
}

char* get_redir_filename(const t_token *start_redir, const t_token *end_redir, int *is_quoted) {
	char *filename;
	char **args;

	if (!start_redir) // if last element with !end_redir do anything specific?
		return (NULL);
	if (start_redir->is_redirection) { //skip redirection symbols --> alreadz handled in get next redir
		if (start_redir->is_redir_heredoc || start_redir->is_redir_output_append)
			start_redir = start_redir->next;
		start_redir = start_redir->next;
	}
	if (end_redir && (end_redir->is_redirection || end_redir->is_pipe)) //skip back before redirection symbols if included
		end_redir = end_redir->prev;
	*is_quoted = contains_quote(start_redir, end_redir);
	args = ft_split_on_ifs((t_token*) start_redir,(t_token*)end_redir);
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


void  redirection_lst_add_back(t_redirect **redir_head, t_redirect *new_redir) {

	t_redirect *last_elem;

	if (!redir_head || !new_redir)
		return;
	if (*redir_head == NULL)
	{
		*redir_head = new_redir;
		return;
	}
	last_elem = *redir_head;
	while (last_elem->next)
		last_elem = last_elem->next;
	last_elem->next = new_redir;
}

int create_redirection(t_parsing *parser, const t_token *start_redir, const t_token *end_redir) {
	t_redirect *redir;

	if (end_redir)
		printf("creating redirection from: token no %d ('%c') to token no %d ('%c')\n", start_redir->idx, start_redir->c, end_redir->idx, end_redir->c);
	else
	printf("creating redirection from: token no %d ('%c') till EOCommand\n", start_redir->idx, start_redir->c);
	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (1);
	memset(redir, 0, sizeof(t_redirect));
	redir->type = get_redir_type(start_redir);
	redir->filename = get_redir_filename(start_redir, end_redir, &(redir->is_quoted));
	if (!redir->filename)
		return (1);
	redirection_lst_add_back(&(parser->current_cmd->redirections), redir);
	return(0);
}

t_token *parse_redirections(t_parsing *parser, const t_token *start_cmd, const t_token *end_cmd, int *parsing_error) {

	t_token *start_first_redir;
	int start_set;
	const t_token *start_redir_tmp;
	const t_token *current;

	if (!start_cmd)
		return (NULL);
	start_set = 0;
	start_first_redir = NULL;
	start_redir_tmp = NULL;
	current = start_cmd;
	while (current && current != end_cmd) {
		if (current->is_redirection) {
			if (!start_first_redir)
				start_first_redir = (t_token*) current;
			if (!start_set) {
				start_set = 1;
				if (current->is_redir_heredoc || current->is_redir_output_append)
					current = current->next;
				start_redir_tmp = current;
			} else { // in case redir is followed by another
				*parsing_error += create_redirection(parser, start_redir_tmp, current);
				start_set = 0;
				continue;
			}
		}
		current = current->next;
	}
	if (start_set) // in case redir goes till EO Command
		*parsing_error += create_redirection(parser, start_redir_tmp, (t_token *)end_cmd);
	if (!start_first_redir) // No redirection found
		return (NULL);
	return (start_first_redir);
}

// HEREDOC complexity todo look for quoted argument
//next->is_comment = 0; //todo deal with this in syntax parsing
//next->is_comment_start = 0;


/* UNUSED!!!
 *
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

//update end_token to point to after redirection, update tokens and remove the redirection tokens
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
				// *end_cmd = current;
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

int create_redirs(t_parsing *parser, t_token *start_redir, const t_token *end_cmd) {

	int count_redir;
	t_redirect *redir;
	t_redirect *prev;
	//t_redirect *last_redir;
	//t_token *current;
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

int parse_next_redir_tokens(t_parsing *parser, t_redirect* redir, t_token **start_redir, const t_token *end_cmd) {

	t_token *start_redir_tmp;
	t_token *end_redir_tmp;

	start_redir_tmp = *start_redir;
	end_redir_tmp = NULL;
	redir->type = get_redir_type(start_redir_tmp);
	if (find_next_redir(&start_redir_tmp, &end_redir_tmp, end_cmd))
		return (1);
	redir->filename = get_redir_filename(parser, start_redir_tmp, end_redir_tmp, &(redir->is_quoted));
	if (!redir->filename)
		return (1);
	*start_redir = end_redir_tmp;// set new starting point to old end point
	return (0);
}
// return (1)if no redirection found or pointers not init
// return 0 if redirection marked, set end redir to end of command if only start and no second redir found
// inclusive of next redir token

static int find_next_redir(t_token **start_redir, t_token **end_redir, const t_token *end_cmd) {
	t_token *start_redir_tmp;
	int start_set;

	if (!start_redir || !*start_redir || !end_redir) //todo check double pointer for null?
		return (1);
	*end_redir = NULL;
	start_set = 0;
	start_redir_tmp = *start_redir;
	while (start_redir_tmp && start_redir_tmp != end_cmd) {
		if (start_redir_tmp->is_redirection) {
			if (!start_set) {
				start_set = 1;
				if (start_redir_tmp->is_redir_heredoc || start_redir_tmp->is_redir_output_append)
					start_redir_tmp = start_redir_tmp->next;
				*start_redir = start_redir_tmp;
			} else {
				*end_redir = start_redir_tmp;
				return (0);
			}
		}
		start_redir_tmp = start_redir_tmp->next;
	}
	if (start_set) { // in case redir goes till EO Command
		*end_redir = (t_token *)end_cmd;
		return (0);
	}
	return (1); // No redirection found
}

*/
