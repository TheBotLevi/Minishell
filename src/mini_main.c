/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/30 15:36:27 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_mini_loop(t_mini *mini)
{
	char	*line;
	int	status;

	line = NULL;
	status = 0;
	while (1)
	{
		//setup_signals();
		g_exit = 0;
		line = readline("minishell > ");
//		printf("hello\n");
		if (line == NULL)
		{
			if (g_exit == 130)
			{
				status = 130;
				continue ;
			}
			ft_putendl_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (line && *line)
			add_history(line);
		status = process_command(line, mini);
		update_exit_status(mini);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		printf("%d\n", status);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_mini	*mini;
	(void)	av;

	if (ac != 1) {
		ft_putendl_fd("minishell: too many arguments\n", STDERR_FILENO);
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
