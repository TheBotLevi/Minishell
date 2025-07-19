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


/*returns -1 when end of tokens*/
int get_next_cmd(t_parsing *parser, t_token **cur_token) {
	t_command *cmd;
	t_token *current;
	t_token *cmd_end;
	t_token *start_redir;

	cmd = parser->current_cmd;
	start_redir = NULL;
	cmd_end = NULL;
	if (detect_redir(parser, cur_token, &start_redir, &cmd_end))
		return (1);
	current = *cur_token;
	cmd->argv = ft_split_on_ifs(&current, start_redir);
	if (!cmd->argv) // todo decide what to return wrong or on empty input, i.e. !cmd->argv[0]
		return (-1);
	*cur_token = cmd_end;
	return (0);
}

t_command*	parse_tokens(t_parsing *parser, t_token* token)
{
	t_command	*prev;
	int			i;

	i = 0;
	prev = NULL;
	while (i < parser->n_cmds) {
		parser->current_cmd = malloc(sizeof(t_command));
		if (!parser->current_cmd)
		{
			free_cmds(*(parser->cmd_head));
			return (NULL);
		}
		ft_memset(parser->current_cmd, 0, sizeof(t_command));
		if (i == 0)
			parser->cmd_head = &parser->current_cmd;
		else
			prev->next = parser->current_cmd;
		if (get_next_cmd(parser, &token) == -1) //todo  add option of ending early withuot freeing when commands are invalid//empty
		{
			free(parser->current_cmd);
			free_cmds(*(parser->cmd_head));
			return (NULL);
		}
		prev = parser->current_cmd;
		if (token && token->is_pipe)
			token = token->next;
		i++;
	}
	if (parser->current_cmd)
		parser->current_cmd->next = NULL;
	return (*(parser->cmd_head));
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
