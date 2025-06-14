/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:23:04 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/07 15:09:29 by ljeribha         ###   ########.fr       */
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

static void	handle_invalid_args(char **args)
{
	ft_putstr_fd("minishell: exit ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

int	mini_exit(char **args)
{
	int	exit_code;
	int	ac;

	ac = 0;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit_code = 1;
	else if (!is_numeric(args[1]))
	{
		handle_invalid_args(args);
		exit_code = 2;
	}
	else if (ac > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	else
		exit_code = ft_atoi(args[1]) & 255;
	exit(exit_code);
	return (exit_code);
}
