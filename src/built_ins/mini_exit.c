/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:23:04 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/30 11:02:33 by ljeribha         ###   ########.fr       */
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
	ft_putstr_fd(mini->args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

int	mini_exit(t_mini *mini)
{
	int	arg_count;

	arg_count = 0;
	while (mini->args[arg_count])
		arg_count++;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (arg_count == 1)
		mini->exit_status = 1;
	else if (!is_numeric(mini->args[1]))
	{
		handle_invalid_args(mini);
		mini->exit_status = 2;
	}
	else if (arg_count > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	else
		mini->exit_status = ft_atoi(mini->args[1]) & 255;
	exit(mini->exit_status);
	return (mini->exit_status);
}
