/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:45:50 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/30 15:48:05 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = temp;
	}
}

void	free_command_list(t_mini *cmds)
{
	t_mini *tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free_args(cmds->args);
		free(cmds->old_path);
		free(cmds);
		cmds = tmp;
	}
}

void	free_pipeline(t_pipeline *pipeline)
{
	int	i;

	i = 0;
	if (!pipeline)
		return ;
	free(pipeline->pids);
	free_command_list(pipeline->commands);
	if (pipeline->pipes)
	{
		while (i < pipeline->cmd_count)
			free(pipeline->pipes[i++]);
		free(pipeline->pipes);
	}
	free(pipeline);
}

void	free_everything(t_mini *mini)
{
	if (!mini)
	return ;
	free_args(mini->args);
	free(mini->old_path);
	if (mini->pipes)
		free_pipeline(mini->pipes);
	free_env_list(mini->env_struct);
	free(mini);
}
