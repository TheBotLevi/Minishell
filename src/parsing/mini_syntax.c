/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:07:11 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/11 13:07:21 by mdiederi         ###   ########.fr       */
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
	return (0);
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

int parse_next_redir_tokens(t_redirect** redir, t_token **start_redir, t_token **end_redir) {

	t_redirect *redir_tmp;
	t_token *start_redir_tmp;
	t_token *end_redir_tmp;

	redir_tmp = *redir;
	find_next_redir(&start_redir_tmp, &end_redir_tmp);
	//redir_tmp->filename = start_redir->filename;
	redir_tmp->type = get_redir_type(*start_redir_tmp);
	*redir = redir_tmp;
	*start_redir = start_redir_tmp;// set new starting point to old
	return(0);
}

int create_redirs(t_command **cmd, t_token *start_redir, t_token *end_redir) {

	int count_redir;
	t_redirect *head_redir;
	t_redirect *redir;
	t_redirect *prev;
	t_redirect *last_redir;
	t_token *current;
	//validate_redirections()

	head_redir = NULL;
	prev = NULL;
	count_redir = count_redirs(start_redir, end_redir);
	while (i < count_redir) {
		redir = malloc(sizeof(t_redirect));
		memset(redir, 0, sizeof(t_redirect));
		if (i == 0) {
			head_redir = redir;
			(*cmd)->redirections = redir;
		}
		else
			prev->next = redir;
		if (parse_next_redir_tokens(&redir, &start_redir, &end_redir))
			return(1);
		if (!redir || !end_redir)
			break;
		prev = redir;
	}
	return (0);
}

// HEREDOC complexity todo look for quoted argument
//next->is_comment = 0; //todo deal with this in syntax parsing
//next->is_comment_start = 0;

/*update end_token to point to after redirection, update tokens and remove the redirection tokens*/
int detect_redir(t_command **cmd, t_token **cur_token, t_token **start_redir, t_token **end_cmd) {
	t_token *current;
	int has_redir;

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
	if (has_redir)
		*cur_token = create_redirs(&cmd, start_redir, end_cmd);
	*end_cmd = current;
	return (0);
}

/*returns -1 when end of tokens*/
int get_next_cmd(t_mini *mini, t_command *cmd, t_token **cur_token) {

	t_token *current;
	t_token *cmd_end;
	t_token *start_redir;
	(void)mini;

	start_redir = NULL;
	cmd_end = NULL;
	if (detect_redir(&cmd, cur_token, &start_redir, &cmd_end))
		return (1);
	current = *cur_token;
	cmd->argv = ft_split_on_ifs(&current, start_redir);
	if (!cmd->argv) // todo decide what to return wrong or on empty input, i.e. !cmd->argv[0]
		return (-1);
	*cur_token = cmd_end;
	return (0);
}

t_command*	parse_tokens(t_mini *mini, t_token* token)
{
	t_command	*cmd;
	t_command	*prev;
	t_command	*cmd_head;
	int			i;

	i = 0;
	prev = NULL;
	cmd_head = NULL;
	while (i < mini->n_cmds) {
		cmd = malloc(sizeof(t_command));
		if (!cmd)
		{
			free_cmds(cmd_head);
			return (NULL);
		}
		ft_memset(cmd, 0, sizeof(t_command));
		if (i == 0)
			cmd_head = cmd;
		else
			prev->next = cmd;
		if (get_next_cmd(mini, cmd, &token) == -1) //todo  add option of ending early withuot freeing when commands are invalid//empty
		{
			free(cmd);
			free_cmds(cmd_head);
			return (NULL);
		}
		prev = cmd;
		if (token && token->is_pipe)
			token = token->next;
		i++;
	}
	if (cmd)
		cmd->next = NULL;
	return (cmd_head);
}


int	process_command2(char *line, t_mini *mini)
{
	t_mini	*pipeline;

	if (!line || !*line)
		return (0);
	// Check if command contains pipes
	if (has_pipes(line))
	{
		if (parse_pipeline(line, &pipeline) == 0)
		{
			mini->exit_status = execute_pipeline(pipeline);
			free_pipeline(pipeline);
		}
		else
			mini->exit_status = 1;
		update_exit_status(mini);
		return (mini->exit_status);
	}
	// Handle single command (no pipes)
	mini->args = parse_input(line);
	if (!mini->args || !mini->args[0])
	{
		if (mini->args)
			free_args(mini->args);
		return (0);
	}
	if (is_builtin(mini->args[0]))
		mini->exit_status = handle_builtin(mini);
	else
		mini->exit_status = execute_external_cmd(mini);
	update_exit_status(mini);
	free_args(mini->args);
	return (mini->exit_status);
}
