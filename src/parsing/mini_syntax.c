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
	return 0; // No conflict
}


// todo mark and cmd sep: |, <, >, <<, >> () to split into commands

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

void	free_cmds(t_command **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	args = NULL;
}

/* returns arg length minus quote marks*/
int get_next_arg_len(t_token **token) {
	int len;
	t_token *current;

	len = 0;
	current = *token; //todo simplifie and truly advance the pointer if set later anyways
	while (current && current->is_ifs) //skip leading whitespace
		current = current->next;
	while (current && !current->is_ifs && !current->is_pipe && !current->is_comment_start && !current->is_comment && !current->is_start_quote && !current->is_redirection) {
		current = current->next;
		if (!current->is_start_quote || !current->is_end_quote)
			len++;
	}
	*token = current;
	return (len);
}
/*
int create_redirs(t_command **cmd, t_token *current) {

t_redirect *redirs;
	t_redirect *last_redir;
	t_command **cmd;
	t_token *current;
	//validate_redirections()

	return (0);
}*/

/*returns -1 when end of tokens*/
int get_next_cmd(t_command *cmd, t_token **cur_token) {

	t_token *current;
	t_token *cmd_end;
	//t_token *start_redir;
	int has_redir;
	//int arg_len;

	current = *cur_token;
	//start_redir = NULL;
	has_redir = 0;
	while (current && !current->is_pipe) { //perhaps add comment as stop? except heredoc
		if (current->is_redirection && !has_redir) {
			has_redir = 1;
			//start_redir = current;
			if (current->is_redir_heredoc)
				cmd->is_heredoc = 1;
			/*
			if (create_redirs(&cmd, current))
				return (-1);*/
		}
		current = current->next;
	}
	cmd_end = current;
	current = *cur_token;
	cmd->argv = ft_split_on_ifs(&current, cmd_end);
	if (!cmd->argv || !cmd->argv[0])
		return (-1);
	print_array(cmd->argv);
	//printf("%c\n", current->c);
	/*
	arg_len = 0;
	while (current && current != cmd_end) {
		arg_len = get_next_arg_len(&current);
	}

	//expand vars
	//todo parse_args(current, cmd);
	if (current && current->is_pipe) {
		*cur_token = current;
		return (0);
	}
	*cur_token = cmd_end;
	return (-1);*/
	*cur_token = cmd_end;
	return (0);
}

t_command**	parse_tokens(t_mini *mini, t_token ** tokens)
{
	t_command	*cmd;
	t_command	**cmds;
	int			i;
	t_token* cur_token;

	cmds = malloc((mini->n_cmds + 1) * sizeof(t_command *));
	if (!cmds)
		return (NULL);
	ft_memset(cmds, 0, (mini->n_cmds + 1) * sizeof(t_command *));
	i = 0;
	cur_token = tokens[0];
	while (i < mini->n_cmds) {
		cmd = malloc(sizeof(t_command));
		if (!cmd) {
			free_cmds(cmds);
			return (NULL);
		}
		ft_memset(cmd, 0, sizeof(t_command));
		get_next_cmd(cmd, &cur_token);
		if (cur_token && cur_token->is_pipe) {
			cur_token = cur_token->next;
		}
		cmds[i] = cmd;
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

//todo look for quoted argument
//next->is_comment = 0; //todo deal with this in syntax parsing
//next->is_comment_start = 0;

// todo perform variable substitution

// check for EOL is pipe (starts multiline input) --> invalid command --> check if last cmd has no args

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
