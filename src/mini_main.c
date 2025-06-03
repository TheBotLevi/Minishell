/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:29:23 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/03 08:08:52 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_mini_loop()
{
	char	*line;
	size_t	len;

	len = 0;
	line = NULL;
	while (1)
	{
		if ((line = readline("Minishell > ")) == NULL)
			break ;
		if (line && *line)
			add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		else if (ft_strcmp(line, "pwd") == 0)
			cmd_pwd();
		free(line);
	}
}

int	main()
{
	surpress_rl_leaks();
	ft_mini_loop();
	clear_history();
	rl_clear_history();
	return (0);
}
