/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/20 11:09:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_mini_loop(t_env *env_list)
{
	char	*line;
	int	status;

	line = NULL;
	status = 0;
	while (1)
	{
		line = readline("minishell > ");
		setup_signals();
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		status = process_command(line, &env_list); //todo parse here
		update_exit_status(&env_list, status);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_list;

	(void)ac;
	(void)av;
	env_list = init_environment(envp);
	if (!env_list)
	{
		ft_putendl_fd("Error initializing environment", STDERR_FILENO);
		return (1);
	}
	setup_signals();
	surpress_rl_leaks();
	ft_mini_loop(env_list);
	free_env_list(env_list);
	clear_history();
	rl_clear_history();
	return (0);
}
