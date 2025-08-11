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
