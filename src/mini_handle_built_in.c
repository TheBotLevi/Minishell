/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_built_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:44:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/08 14:53:55 by ljeribha         ###   ########.fr       */
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
	ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

//  127 error code indicates “command not found"
int	handle_builtin(t_mini *mini)
{
	if (!mini->cur_cmd->args || !mini->cur_cmd->args[0])
		return (1);
	if (ft_strcmp(mini->cur_cmd->args[0], "echo") == 0)
		return (mini_echo(mini->cur_cmd->args));
	if (ft_strcmp(mini->cur_cmd->args[0], "cd") == 0)
		return (mini_cd(mini));
	if (ft_strcmp(mini->cur_cmd->args[0], "pwd") == 0)
		return (mini_pwd());
	if (ft_strcmp(mini->cur_cmd->args[0], "export") == 0)
		return (mini_export(mini));
	if (ft_strcmp(mini->cur_cmd->args[0], "unset")== 0)
		return (mini_unset(mini));
	if (ft_strcmp(mini->cur_cmd->args[0], "env") == 0)
		return (mini_env(&mini->env_struct));
	if (ft_strcmp(mini->cur_cmd->args[0], "exit") == 0)
		return (mini_exit(mini));
	return (127);
}
