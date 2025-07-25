/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/18 13:27:20 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


t_command *test_parsing(char* line, t_mini* mini) {
	t_parsing *parser;
	t_command*	cmds;
	t_command*	cmds_return;
	t_redirect*	redir_head;

	parser = malloc(sizeof(t_parsing));
	if (!parser)
		return (NULL);
	memset(parser, 0, sizeof(t_parsing));
	parser->ifs = set_ifs(mini);
	parser->env_struct = mini->env_struct;
	parser->exit_status = mini->exit_status;
	//printf("\nInput tokens\n----\n");
	parser->tokens_head = tokenize(line, parser);
	if (!parser->tokens_head) {
		printf("Error tokenizing\n");
		return (NULL);
	}
	print_tokens(parser->tokens_head);
	if (parse_tokens(parser) > 0) {
		printf("Error parsing commands\n");
		free_tokens(parser->tokens_head);
		if (parser->cmd_head)
			free_cmds(parser->cmd_head);
		free(parser);
		return (NULL);
	}
	cmds = parser->cmd_head;
	cmds_return = parser->cmd_head;
	while (cmds && cmds->argv){
		print_array(cmds->argv);
		redir_head = cmds->redirections;
		while (redir_head) {
			printf("filename: %s, type: %d\n", redir_head->filename, redir_head->type);
			redir_head = redir_head->next;
		}
		cmds=cmds->next;
	}
	//printf("----\n");
	fflush(stdout);
	free_tokens(parser->tokens_head);
	//free_cmds(parser->cmd_head);
	free(parser);
	return (cmds_return);
}

void	ft_mini_loop(t_mini *mini)
{
	char	*line;
	int	status;
	t_command	*cmds;

	line = NULL;
	status = 0;
	while (1)
	{
		setup_signals();
		g_exit = 0;
		line = readline("mariashell > ");
//		printf("hello\n");
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
		cmds = test_parsing(line, mini);
		status = process_command(line, mini);
		if (status == 130 || g_exit == 130)
		{
			mini->exit_status = 130;
			g_exit = 0;
		}
		update_exit_status(mini);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
//		printf("exit status: %d\n", status);
		free(line);
		free(cmds);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_mini	*mini;
	(void)	av;

	if (ac != 1) {
		ft_putendl_fd("mariashell: too many arguments", STDERR_FILENO);
		return (1);
	};
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
