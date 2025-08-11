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

// add new command struct to the end of a cmd list or set as new head
// if list is empty
void	command_lst_add_back(t_command **cmd_head, t_command *new_cmd)
{
	t_command	*last_elem;

	if (!cmd_head || !new_cmd)
		return ;
	if (*cmd_head == NULL)
	{
		*cmd_head = new_cmd;
		return ;
	}
	last_elem = *cmd_head;
	while (last_elem->next)
		last_elem = last_elem->next;
	last_elem->next = new_cmd;
}

/*returns 1 when malloc issue
 * updates **cmd start to new cmd start in the end
 */
int	get_next_cmd(t_parsing *parser, t_token **cmd_start)
{
	t_token	*cmd_end;

	if (*cmd_start == NULL)
		return (0);
	cmd_end = get_cmd_end(*cmd_start);
	if (parse_redirections(parser, *cmd_start, cmd_end)){
		return (1);
	}
	parser->current_cmd->args = ft_split_on_ifs(*cmd_start, cmd_end, 1);
	*cmd_start = cmd_end;
	while (*cmd_start && (*cmd_start)->is_pipe)
		*cmd_start = (*cmd_start)->next;
	return (0);
}

// sets all flags except "has heredoc" as this is checked in each redirection
// (if multiple) and set if ANY ONE has it
void	set_cmd_flags(t_parsing *parser)
{
	t_command	*current;

	current = parser->cmd_head;
	if (!current)
		return ;
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

int	parse_tokens(t_parsing *parser, t_mini *mini)
{
	t_token	*cmd_start;
	int		i;

	i = 0;
	cmd_start = parser->tokens_head;
	mini->cmd_count = parser->n_cmds;
	while (i < parser->n_cmds && cmd_start)
	{
		parser->current_cmd = malloc(sizeof(t_command));
		if (!parser->current_cmd)
			return (1);
		ft_memset(parser->current_cmd, 0, sizeof(t_command));
		command_lst_add_back(&(parser->cmd_head), parser->current_cmd);
		if (get_next_cmd(parser, &cmd_start))
			return (2);
		if ((!parser->current_cmd->args || !parser->current_cmd->args[0])
			&& !parser->current_cmd->has_redir) //todo should be permitted if redirection is present?
		{
			if (parser->n_cmds > 1)
				return (3);
			return (4); //todo write error message?
		}
		i++;
	}
	set_cmd_flags(parser);
	return (0);
}
