/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 08:27:43 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/10 08:27:45 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// prints error and frees tokens
void	print_unexpected_token_error(const t_token *token)
{
	if (token)
	{
		ft_putstr_fd("mariashell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putchar_fd(token->c, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);
	}
	else
		ft_putendl_fd("mariashell: syntax error near unexpected token"
			" `newline'",
			STDERR_FILENO);
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("[%d] %c: ifs:%d, quote:%d, \':%d, \":%d, start\":%d, "
			"end\":%d,|:%d, $:%d, $?:%d, $var:%d, <:%d,"
			" <<:%d, <<_del: %d, >:%d, >>:%d, filen:%d \n",
			current->idx,
			current->c, current->is_ifs,
			current->is_quote, current->is_single_quote,
			current->is_double_quote, current->is_start_quote,
			current->is_end_quote, current->is_pipe,
			current->is_dollar, current->is_exit_status,
			current->is_var, current->is_redir_input,
			current->is_redir_heredoc, current->is_redir_heredoc_delimiter,
			current->is_redir_output, current->is_redir_output_append,
			current->is_redir_filename);
		current = current->next;
	}
}

void	print_commands(t_command *commands)
{
	t_command	*cmds;
	t_redirect	*redir_head;

	cmds = commands;
	while (cmds && cmds->args)
	{
		print_array(cmds->args);
		redir_head = cmds->redirections;
		while (redir_head)
		{
			printf("filename/ delim: %s, type: %d, quoted: %d\n",
				redir_head->filename, redir_head->type, redir_head->is_quoted);
			redir_head = redir_head->next;
		}
		cmds = cmds->next;
	}
}
