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

/*returns 1 when end of tokens
 * updates **cmd start to new cmd start in the end
 */
int get_next_cmd(t_parsing *parser, t_token **cmd_start) {
	t_token *cmd_end;
	t_token *start_redir;
	int has_redir;

	start_redir = NULL;
	cmd_end = get_cmd_end(*cmd_start); //todo what if cmd_end == start?
	if (cmd_end)
		printf("end cmd: %c", cmd_end->c);
	has_redir = detect_redir(parser, *cmd_start, &start_redir, cmd_end);
	if (start_redir)
		printf("start redir: %c", start_redir->c);
	if (has_redir > 0) {
		if (create_redirs(parser, start_redir, cmd_end)) //todo have to update start_pointer in here?
			return (1);
	}
	else return (1); //todo what to do on  has_redir == -1?
	parser->current_cmd->argv = ft_split_on_ifs(*cmd_start, start_redir);
	*cmd_start = cmd_end; //set new cmd start
	if (*cmd_start && (*cmd_start)->is_pipe)
		*cmd_start = (*cmd_start)->next;
	if (!parser->current_cmd->argv) // todo decide what to return wrong or on empty input, i.e. !cmd->argv[0]
		return (1);
	return (0);
}

int parse_tokens(t_parsing *parser)
{
	t_command	*prev;
	t_token	*cmd_start;
	int			i;

	i = 0;
	prev = NULL;
	cmd_start = parser->tokens_head;
	while (i < parser->n_cmds) {
		parser->current_cmd = malloc(sizeof(t_command));
		if (!parser->current_cmd)
		{
			free_cmds(parser->cmd_head);
			return (1);
		}
		ft_memset(parser->current_cmd, 0, sizeof(t_command));
		if (i == 0)
			parser->cmd_head = parser->current_cmd;
		else
			prev->next = parser->current_cmd;
		if (get_next_cmd(parser, &cmd_start)) //todo add option of ending early without freeing when commands are invalid//empty
		{
			free(parser->current_cmd);
			free_cmds(parser->cmd_head);
			return (1);
		}
		if (cmd_start)
			printf("cur token, new start: %c", cmd_start->c);
		prev = parser->current_cmd;

		i++;
	}
	if (parser->current_cmd)
		parser->current_cmd->next = NULL;
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
