/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:45:50 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/18 13:13:01 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env_array(char **array, int count)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (i < count)
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}

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

void	help_free_pipelines(t_mini *pipeline)
{
	int	i;

	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		free(pipeline->pipes[i]);
		i++;
	}
	free(pipeline->pipes);
}

void	free_everything(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmds)
		free_cmds(mini->cmds);
	free(mini->old_path);
	if (mini->pids)
		free(mini->pids);
	if (mini->pipes)
		help_free_pipelines(mini);
	if (mini->env_struct)
		free_env_list(mini->env_struct);
	if (mini->envp)
		free_args(mini->envp);
	free(mini);
}
