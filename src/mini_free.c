/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:45:50 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/04 12:48:15 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_n_array(char **ar, size_t i)
{
	while (i > 0)
		free(ar[--i]);
	free(ar);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
//		printf("Hello %d\n", i);
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
//		free_args(cmds->args);
		free(cmds->old_path);
		free(cmds);
		cmds = tmp;
	}
}

static void	help_free_pipelines(t_mini *pipeline)
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

void	free_pipeline(t_mini *pipeline)
{
	t_mini	*current;
	t_mini	*next;

	if (!pipeline)
		return;
	current = pipeline->commands;
	while (current)
	{
		next = current->next;
		free_args(current->args);
		free(current);
		current = NULL;
		current = next;
	}
	if (pipeline->pipes)
		help_free_pipelines(pipeline);
	if (pipeline->pids)
		free(pipeline->pids);
//	if (pipeline->commands)
//		free(pipeline->commands);
	free(pipeline);
}

void	free_everything(t_mini *mini)
{
	if (!mini)
	return ;
//	free_args(mini->args);
	free(mini->old_path);
	if (mini->pipes)
		free_pipeline(mini);
	free_env_list(mini->env_struct);
	free(mini);
}
