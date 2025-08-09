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
	//t_token *start_redir;
	//int has_redir;
	int parsing_error;

	if (*cmd_start == NULL)
		return (0);
	//start_redir = NULL;
	cmd_end = get_cmd_end(*cmd_start); //todo what if cmd_end == start?
	if (cmd_end)
		printf("end cmd: %c\n", cmd_end->c);
	parsing_error = 0;
	//start_redir =parse_redirections(parser, (const t_token*) *cmd_start, (const t_token*) cmd_end, &parsing_error);
	parse_redirections(parser, (const t_token*) *cmd_start, (const t_token*) cmd_end, &parsing_error);
	/*if (parsing_error)
		return (parsing_error);*/
	/*if (!start_redir)
		start_redir = cmd_end;*/
	//start_redir = cmd_end;
	parser->current_cmd->args = ft_split_on_ifs(*cmd_start, cmd_end, 1);
	*cmd_start = cmd_end; //set new cmd start
	while (*cmd_start && (*cmd_start)->is_pipe)
		*cmd_start = (*cmd_start)->next;
	return (0);
}

// sets all flags except "has heredoc" as this is checked in each redirection (if mu]ltiple) and set if ANY ONE has it
void set_cmd_flags(t_parsing *parser) {
	t_command	*current;

	current = parser->cmd_head;
	if (!current)
		return;

	while (current)
	{
		if (current->next)
			current->has_pipe_out = 1;
		if (current != parser->cmd_head)
			current->has_pipe_in = 1;
		if (current->redirections)
			current->has_redir = 1;
		if (current->args && is_builtin(current->args[0]))
			current->is_builtin = 1;
		current->input_fd = -1;
		current->output_fd = -1;
		current->env_struct = parser->env_struct;
		current = current->next;
	}
}

int parse_tokens(t_parsing *parser, t_mini *mini)
{
	//t_command	*prev;
	t_token	*cmd_start;
	int			i;

	i = 0;
	//prev = NULL;
	cmd_start = parser->tokens_head;
	mini->cmd_count = parser->n_cmds;
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
		if (!parser->current_cmd->args || !parser->current_cmd->args[0]) {
			if (parser->n_cmds > 1)
				ft_putendl_fd("mariashell: syntax error near unexpected token `|'", 2);
			return (1);
		}
		if (cmd_start)
			printf("cur token, new start: %c (%d)\n", cmd_start->c, cmd_start->idx);
		i++;
	}
	set_cmd_flags(parser);
	return (0);
}