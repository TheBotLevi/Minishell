/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:40:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/05/20 08:44:22 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	surpress_rl_leaks(void)
{
	rl_clear_history();
	rl_readline_name = "Minishell";
}

void	cmd_pwd(void)
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE) == NULL)
	{
		printf("Error: cannot get current working directory path!\n");
		return ;
	}
	printf("%s\n", buffer);
}
