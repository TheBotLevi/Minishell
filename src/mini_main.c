/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/13 16:16:37 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// error 1 has no specific message (e.g. empty line)
// or message already printed in the function
int	handle_tokenize_result(t_mini *mini, t_parsing *parser, int token_error)
{
	if (token_error > 0)
	{
		if (token_error == 2)
		{
			ft_putendl_fd("minishell: memory allocation error during "
				"token creation", STDERR_FILENO);
			mini->exit_status = 1;
		}
		if (token_error == 3)
		{
			ft_putendl_fd("minishell: memory allocation error during "
				"variable expansion", STDERR_FILENO);
			mini->exit_status = 1;
		}
		if (token_error == 4)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token"
				" `|'", 2);
			mini->exit_status = 2;
		}
		parser->tokens_head = NULL;
		free(parser);
	}
	return (token_error);
}

t_command	*parse_line_to_commands(char *line, t_mini *mini)
{
	t_parsing	*parser;
	t_command	*cmds;

	cmds = NULL;
	parser = init_parser(mini);
	if (!parser || !line)
		return (NULL);
	if (handle_tokenize_result(mini, parser, tokenize(line, parser)))
		return (NULL);
	mini->cmd_count = parser->n_cmds;
	cmds = handle_parsing_result(mini, parser, parse_tokens(parser));
	return (cmds);
}

static int	handle_line(t_mini *mini, char *line)
{
	int	status;

	if (!*line)
	{
		free(line);
		return (0);
	}
	add_history(line);
	status = process_command(mini, line);
	if (status == 130 || g_exit == 130)
	{
		free(line);
		return (130);
	}
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

void	ft_mini_loop(t_mini *mini)
{
	char	*line;
	int		status;

	line = NULL;
	status = 0;
	while (1)
	{
		setup_signals();
		g_exit = 0;
		line = readline("minishell > ");
		if (line == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (g_exit == 130)
			g_exit = 0;
		status = handle_line(mini, line);
		if (status == -1)
			break ;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_mini	*mini;

	(void)av;
	if (ac != 1)
	{
		ft_putendl_fd("minishell: too many arguments", STDERR_FILENO);
		return (1);
	}
	mini = mini_init(envp);
	if (!mini)
		return (1);
	setup_signals();
	ft_mini_loop(mini);
	rl_clear_history();
	free_everything(mini);
	return (0);
}
