/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:24:36 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/27 11:17:11 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	only_n(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	mini_echo(char **cmds)
{
	int	i;
	int	new_line;
	int	printed;

	i = 1;
	printed = 0;
	new_line = 1;
	while (cmds[i] && cmds[i][0] == '-' && cmds[i][1] == 'n'
		&& only_n(&cmds[i][1]))
	{
		new_line = 0;
		i++;
	}
	while (cmds[i])
	{
		ft_putstr_fd(cmds[i], STDOUT_FILENO);
		if (cmds[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
		printed = 1;
	}
	if (new_line && printed)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
