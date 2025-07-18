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


void test_parsing(char* line, t_mini* mini) {
	t_parsing *parser;
	t_token **tokens;
	t_command*	cmds;
	t_command*	cmd_head;

	parser = malloc(sizeof(t_parsing));
	if (!parser)
		return ;
	memset(parser, 0, sizeof(t_parsing));
	parser->ifs = set_ifs(mini);
	parser->env_struct = mini->env_struct;
	parser->exit_status = mini->exit_status;
	//printf("\nInput tokens\n----\n");
	tokens = tokenize(line, parser);
	if (!tokens)
		return ;
	print_tokens(*tokens);
	cmds = parse_tokens(parser, *tokens);
	cmd_head = cmds;
	while (cmds && cmds->argv){
		print_array(cmds->argv);
		while (cmds->redirections) {
			printf("filename: %s, type: %d\n", cmds->redirections->filename, cmds->redirections->type);
			cmds->redirections = cmds->redirections->next;
		}
		cmds=cmds->next;
	}
	//printf("----\n");
	fflush(stdout);
	free_tokens(tokens);
	free_cmds(cmd_head);
	free(parser);
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
		//test_parsing(line, mini);
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
