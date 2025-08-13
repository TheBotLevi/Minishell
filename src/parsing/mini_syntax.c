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

/*returns 1 when malloc issue
 * updates **cmd start to new cmd start in the end
 */
int	get_next_cmd(t_parsing *parser, t_token **cmd_start)
{
	t_token	*cmd_end;
	char	**args;
	int		redir_error;

	if (*cmd_start == NULL)
		return (0);
	cmd_end = get_cmd_end(*cmd_start);
	redir_error = parse_redirections(parser, *cmd_start, cmd_end);
	if (redir_error)
		return (redir_error);
	args = ft_split_on_ifs(*cmd_start, cmd_end, 1);
	parser->current_cmd->args = args;
	*cmd_start = cmd_end;
	while (*cmd_start && (*cmd_start)->is_pipe)
		*cmd_start = (*cmd_start)->next;
	return (0);
}


void heredoc_lst_add_back(t_heredoc **heredoc_head, t_heredoc *new_heredoc)
{
	t_heredoc	*last_elem;

	if (!heredoc_head || !new_heredoc)
		return ;
	if (*heredoc_head == NULL)
	{
		*heredoc_head = new_heredoc;
		return ;
	}
	last_elem = *heredoc_head;
	while (last_elem->next)
		last_elem = last_elem->next;
	last_elem->next = new_heredoc;
}

create_heredoc(t_parsing *parser, t_redirect	*redir) {

	t_heredoc	*heredoc;

	heredoc = NULL;
	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (1);
	ft_memset(heredoc, 0, sizeof(t_heredoc));
	heredoc->redir = redir;
	heredoc->delimiter = redir->filename;
	heredoc->quoted = redir->is_quoted;
	heredoc_lst_add_back((&parser->heredoc), heredoc);
	return (0);
}

// sets all flags except "has heredoc" as this is checked in each redirection
// (if multiple) and set if ANY ONE has it
int	set_cmd_flags(t_parsing *parser)
{
	t_command	*current;
	t_redirect	*redir;

	redir = NULL;
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
		{
			current->has_redir = 1;
			if (current->has_heredoc){
				while (redir) {
					if (redir->type == REDIR_HEREDOC){
						if (create_heredoc(parser, redir))
							return (1);
					}
					redir = redir->next;
				}
			}
		}
		if (current->args && is_builtin(current->args[0]))
			current->is_builtin = 1;
		current->input_fd = -1;
		current->output_fd = -1;
		current->env_struct = parser->env_struct;
		current = current->next;
	}
	return (0);
}

int	parse_tokens(t_parsing *parser)
{
	t_token	*cmd_start;
	int		i;
	int		cmd_error;

	i = 0;
	cmd_start = parser->tokens_head;
	while (i < parser->n_cmds && cmd_start)
	{
		parser->current_cmd = malloc(sizeof(t_command));
		if (!parser->current_cmd)
			return (1);
		ft_memset(parser->current_cmd, 0, sizeof(t_command));
		command_lst_add_back(&(parser->cmd_head), parser->current_cmd);
		cmd_error = get_next_cmd(parser, &cmd_start);
		if (cmd_error)
			return (cmd_error);
		i++;
	}
	if (set_cmd_flags(parser) > 0)
		return (1);
	return (0);
}
