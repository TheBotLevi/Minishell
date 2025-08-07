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

// add new command struct to the end of a cmd list or set as new head if list is empty
void  command_lst_add_back(t_command **cmd_head, t_command *new_cmd) {

	t_command *last_elem;

	if (!cmd_head || !new_cmd)
		return;
	if (*cmd_head == NULL)
	{
		*cmd_head = new_cmd;
		return;
	}
	last_elem = *cmd_head;
	while (last_elem->next)
		last_elem = last_elem->next;
	last_elem->next = new_cmd;
}

/*returns 1 when end of tokens
 * updates **cmd start to new cmd start in the end
 */
int get_next_cmd(t_parsing *parser, t_token **cmd_start) {
	t_token *cmd_end;
	t_token *start_redir;
	//int has_redir;
	int parsing_error;

	if (*cmd_start == NULL)
		return (0);
	start_redir = NULL;
	cmd_end = get_cmd_end(*cmd_start); //todo what if cmd_end == start?
	if (cmd_end)
		printf("end cmd: %c\n", cmd_end->c);
	parsing_error = 0;
	start_redir = parse_redirections(parser, (const t_token*) *cmd_start, (const t_token*) cmd_end, &parsing_error);  // todo what to do on multiple redirs?
	if (parsing_error)
		return (parsing_error);
	if (!start_redir)
		start_redir = cmd_end;
	parser->current_cmd->argv = ft_split_on_ifs(*cmd_start, start_redir);
	*cmd_start = cmd_end; //set new cmd start
	while (*cmd_start && (*cmd_start)->is_pipe)
		*cmd_start = (*cmd_start)->next;
	if (!parser->current_cmd->argv) // todo decide what to return wrong or on empty input, i.e. !cmd->argv[0]  // case heredoc without prev argument permissible
		return (1);
	return (0);
}

// sets all flags except "has heredoc" as this is checked in each redirection (if mu]ltiple) and set if ANY ONE has it
void set_cmd_flags(t_parsing *parser) {

	t_command *current;

	current = parser->cmd_head;
	if (!current)
		return;
	if (current->next) {
		current->has_pipe_out = 1;
		if (current->redirections)
			current->has_redir = 1;
		current->has_pipe_out = 1;
		//todo if () current->is_builtin = 1;
		current = current->next;
	}
	while (current) {
		if (current->redirections)
			current->has_redir = 1;
		current->has_pipe_in = 1;
		if (current->next)
			current->has_pipe_out = 1;
		//todo if () current->is_builtin = 1;
		current = current->next;
	}

}

int parse_tokens(t_parsing *parser)
{
	//t_command	*prev;
	t_token	*cmd_start;
	int			i;

	i = 0;
	//prev = NULL;
	cmd_start = parser->tokens_head;
	printf("n comamnds: %d\n", parser->n_cmds);
	while (i < parser->n_cmds && cmd_start) {
		parser->current_cmd = malloc(sizeof(t_command));
		if (!parser->current_cmd)
		{
			free_cmds(parser->cmd_head);
			return (1);
		}
		ft_memset(parser->current_cmd, 0, sizeof(t_command));
		command_lst_add_back(&(parser->cmd_head), parser->current_cmd);
		printf("cmd start: %d\n", cmd_start->idx);
		if (get_next_cmd(parser, &cmd_start)) //todo add option of ending early without freeing when commands are invalid//empty
			return (1);
		if (cmd_start)
			printf("cur token, new start: %c (%d)\n", cmd_start->c, cmd_start->idx);
		i++;
	}
	set_cmd_flags(parser);
	return (0);
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
