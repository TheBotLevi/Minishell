/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_built_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:44:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/27 16:53:30 by ljeribha         ###   ########.fr       */
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

int	handle_builtin(char **cmd, t_env **env)	//is going to change
{
	t_cmd	command;

	if (!cmd || !cmd[0])
		return (1);
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (mini_echo(cmd));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (mini_cd(cmd, env));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (mini_pwd());
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (mini_export(cmd, env));
	else if (ft_strcmp(cmd[0], "unset")== 0)
		return (mini_unset(cmd, env));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (mini_env(env));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (mini_exit(cmd));
	else if (ft_strcmp(cmd[0], "$?") == 0)
		return (command.exit_status);
	return (127);
}
