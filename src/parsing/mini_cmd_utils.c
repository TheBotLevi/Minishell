/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:13:41 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/18 16:13:48 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*get_cmd_end(t_token *cmd_start)
{
	t_token	*current;

	current = cmd_start;
	while (current && !current->is_pipe)
		current = current->next;
	return (current);
}

void	redir_lst_add_back(t_redirect **redir_head, t_redirect *new_redir)
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

t_command	*handle_parsing_result(t_mini *mini, t_parsing *parser, int error)
{
	t_command	*cmds;

	cmds = NULL;
	free_tokens(parser->tokens_head);
	if (error > 0)
	{
		mini->exit_status = 1;
		if (error == 1 || error == 2)
		{
			ft_putendl_fd("mariashell: memory allocation error during "
				"parsing", STDERR_FILENO);
		}
		if (error == 3)
			mini->exit_status = 2;
		if (parser->cmd_head)
			free_cmds(parser->cmd_head);
	}
	else
	{
		cmds = parser->cmd_head;
		free(parser);
	}
	return (cmds);
}

t_parsing	*init_parser(t_mini *mini)
{
	t_parsing	*parser;

	parser = malloc(sizeof(t_parsing));
	if (!parser)
		return (NULL);
	memset(parser, 0, sizeof(t_parsing));
	parser->ifs = set_ifs(mini);
	parser->env_struct = mini->env_struct;
	parser->exit_status = mini->exit_status;
	return (parser);
}
