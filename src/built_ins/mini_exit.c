/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:23:04 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/13 17:26:19 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_invalid_args(t_mini *mini)
{
	ft_putstr_fd("minishell: exit ", STDERR_FILENO);
	ft_putstr_fd(mini->cur_cmd->args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	free_everything(mini);
	exit(2);
}

/*
Cases:
- No arguments → exit with current mini->exit_status
- One numeric argument → exit with that value modulo 256
- One non-numeric argument OR two or more arguments, first is non-numeric
		→ print numeric argument required, exit 2
- Two or more arguments, first is numeric → print too many arguments,
	don’t exit, status = 1
 */
int	mini_exit(t_mini *mini)
{
	int	argc;
	int	exit_status;

	argc = 0;
	while (mini->cur_cmd->args[argc])
		argc++;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (argc == 1)
	{
		exit_status = mini->exit_status;
		free_everything(mini);
		exit(exit_status);
	}
	if (!is_numeric(mini->cur_cmd->args[1]))
		handle_invalid_args(mini);
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	exit_status = ft_atoi(mini->cur_cmd->args[1]) & 255;
	free_everything(mini);
	exit(exit_status);
}
