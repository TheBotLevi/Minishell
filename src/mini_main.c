/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/28 13:40:49 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int has_line_continuation(char *line) {
	if (line && *line)
		if (line[ft_strlen(line) - 1] == '\\')
			return (1);
	return (0);
}
void fill_line_with_multiline_input(char **line) {
	char *multiline_input;

	multiline_input = ft_strjoin(*line, readline("> "));
	free(*line);
	*line = multiline_input;
}

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
		while (has_line_continuation(line))
			fill_line_with_multiline_input(&line);
		printf("%s\n", line);
		if (line && *line)
			add_history(line);
		//status = process_command(line, mini); //TODO
		status = process_command(line, mini); //TODO
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

int	main(int ac, char **av)
{
	t_mini	*mini;
	(void)	av;

	mini = NULL;
	if (ac != 1) {
		ft_putendl_fd("minishell: too many arguments\n", STDERR_FILENO);
		return (1);
	};
	//mini_init here
	setup_signals();
	surpress_rl_leaks();
	ft_mini_loop(mini);
	free_env_list(mini->env_struct);
	clear_history();
	rl_clear_history();
	return (0);
}
