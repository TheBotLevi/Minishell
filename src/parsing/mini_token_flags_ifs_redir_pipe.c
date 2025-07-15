/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_token_flags_ifs_redir_pipe.c					:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 14:54:51 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/11 14:54:54 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_ifs_flags(t_mini *mini, t_token **tokens)
{
	t_token		*current;
	char		*ifs;
	static char	default_ifs[4];

	default_ifs[0] = ' ';
	default_ifs[1] = '\t';
	default_ifs[2] = '\n';
	default_ifs[3] = '\0';
	ifs = default_ifs;
	if (mini && mini->env_struct && get_env_value(mini->env_struct, "IFS"))
		ifs = get_env_value(mini->env_struct, "IFS");
	current = *tokens;
	while (current && !current->is_comment)
	{
		if (is_in_set(current->c, ifs) && !current->is_quote)
			current->is_ifs = 1;
		current = current->next;
	}
}

int	set_pipe_flags(t_token **tokens)
{
	t_token	*current;
	int n_pipes;

	n_pipes = 0;
	current = *tokens;
	while (current && !current->is_comment)
	{
		if (current->c == '|' && !current->is_quote) {
			current->is_pipe = 1;
			n_pipes++;
		}
		current = current->next;
	}
	return (n_pipes);
}

/*
< should redirect input
> should redirect output
>> should redirect output in append mode.
<< should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
 */
void	set_is_redirection_flag(t_token **head)
{
	t_token	*current;

	current = *head;
	while (current && !current->is_comment)
	{
		if (!current->is_quote && (current->is_redir_output
				|| current->is_redir_input || current->is_redir_heredoc
				|| current->is_redir_output_append))
			current->is_redirection = 1;
		current = current->next;
	}
}

void set_double_redir_flags(t_token	**current) {

	if ((*current)->c == '>')
	{
		(*current)->is_redir_output_append = 1;
		(*current)->next->is_redir_output_append = 1;

	}
	else if ((*current)->c == '<')
	{
		(*current)->is_redir_heredoc = 1;
		(*current)->next->is_redir_heredoc = 1;
	}
	(*current) = (*current)->next;
}

void	set_redirection_flags(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && !current->is_comment)
	{
		if (!current->is_quote)
		{
			if (current->c == '>' && current->next && current->next->c == '>')
				set_double_redir_flags(&current);
			else if (current->c == '<' && current->next && current->next->c == '<')
				set_double_redir_flags(&current);
			else if (current->c == '>')
				current->is_redir_output = 1;
			else if (current->c == '<')
				current->is_redir_input = 1;
		}
		current = current->next;
	}
}
