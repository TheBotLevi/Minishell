/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/09 15:16:18 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_parsing *init_parser(t_mini* mini)
{
	t_parsing *parser;

	parser = malloc(sizeof(t_parsing));
	if (!parser)
		return (NULL);
	memset(parser, 0, sizeof(t_parsing));
	parser->ifs = set_ifs(mini);
	parser->env_struct = mini->env_struct;
	parser->exit_status = mini->exit_status;
	return (parser);
}

t_command*	handle_parsing_result(t_mini *mini, t_parsing *parser, int parse_error) {

	t_command*	cmds;

	cmds = NULL;
	if (parse_error == 1) {
		ft_putendl_fd("mariashell: memory allocation error during "
				" parsing", STDERR_FILENO);
	}
	if (parse_error == 2) {
		ft_putendl_fd("mariashell: memory allocation error during "
				" redirection parsing", STDERR_FILENO);
	}
	if (parse_error == 3) {
		ft_putendl_fd("mariashell: syntax error near unexpected token"
					" `|'", 2);
	}
	/*
	if (parse_error == 4) {
		//ft_putendl_fd("WHAT ERROR?", 2); //todo
	}*/
	free_tokens(parser->tokens_head);
	if (parse_error > 0) {
		mini->exit_status = 2;
		if (parser->cmd_head)
			free_cmds(parser->cmd_head);
	}
	else {
		cmds = parser->cmd_head;
		free(parser);
	}
	return (cmds);
}

static t_command *parse_line_to_commands(char* line, t_mini* mini) {
	t_parsing *parser;
	t_command*	cmds;

	cmds = NULL;
	parser = init_parser(mini);
	if (!parser || !line)
		return (NULL);
	parser->tokens_head = tokenize(line, parser);
	if (!parser->tokens_head) {
		mini->exit_status = 2; //todo hcheck exit status in main?
		free(parser);
		return (NULL);
	}
	print_tokens(parser->tokens_head);
	printf("n comamnds: %d\n", parser->n_cmds);
	cmds = handle_parsing_result(mini, parser, parse_tokens(parser, mini));
	print_commands(cmds);
	return (cmds);
}

void	ft_mini_loop(t_mini *mini)
{
	char	*line;
	int	status;

	line = NULL;
	status = 0;
	while (1)
	{
		setup_signals();
		g_exit = 0;
		line = readline("mariashell > ");
		if (line == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (g_exit == 130)
		{
//			ft_putchar_fd('\n', STDOUT_FILENO);
			g_exit = 0;
//			continue ;
		}
		if (!*line)
		{
			free(line);
			continue ;
		}
		add_history(line);
		mini->cmds = parse_line_to_commands(line, mini);
		status = process_command(mini);
		if (status == 130 || g_exit == 130)
		{
			mini->exit_status = 130;
			g_exit = 0;
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
//		printf("exit status: %d\n", status);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_mini	*mini;
	(void)	av;

	if (!isatty(STDIN_FILENO)) {
		ft_putendl_fd("Warning: Non-interactive mode detected (input is not a terminal).", STDERR_FILENO);
		return (0);
	}
	if (ac != 1) {
		ft_putendl_fd("mariashell: too many arguments", STDERR_FILENO);
		return (1);
	}
	mini = mini_init(envp);
	if (!mini)
		return (1);
	setup_signals();
	ft_mini_loop(mini);
	clear_readline_history();
//	free_env_list(mini->env_struct);
//	clear_history();
//	rl_clear_history();
	free_everything(mini);
	return (0);
}
