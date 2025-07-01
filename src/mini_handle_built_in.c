/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_built_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:44:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/01 15:49:45 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || 
	ft_strcmp(cmd, "cd") == 0 || 
	ft_strcmp(cmd, "pwd") == 0 || 
	ft_strcmp(cmd, "export") == 0 || 
	ft_strcmp(cmd, "unset") == 0 || 
	ft_strcmp(cmd, "env") == 0 || 
	ft_strcmp(cmd, "exit") == 0 ||
	ft_strcmp(cmd, "$?") == 0)
		return (1);
	return (0);
}

int	handle_builtin(t_mini *mini)	//is going to change
{
	if (!mini->args || !mini->args[0])
		return (1);
	if (ft_strcmp(mini->args[0], "echo") == 0)
		return (mini_echo(mini->args));
	else if (ft_strcmp(mini->args[0], "cd") == 0)
		return (mini_cd(mini));
	else if (ft_strcmp(mini->args[0], "pwd") == 0)
		return (mini_pwd());
	else if (ft_strcmp(mini->args[0], "export") == 0)
		return (mini_export(mini));
	else if (ft_strcmp(mini->args[0], "unset")== 0)
		return (mini_unset(mini));
	else if (ft_strcmp(mini->args[0], "env") == 0)
		return (mini_env(&mini->env_struct));
	else if (ft_strcmp(mini->args[0], "exit") == 0)
		return (mini_exit(mini));
	else if (ft_strcmp(mini->args[0], "$?") == 0)
		return (mini->exit_status);
	return (127);
}
