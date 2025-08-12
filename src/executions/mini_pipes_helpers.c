/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:52:25 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/12 12:52:27 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_all_pipes(t_mini *pipeline)
{
	int	i;

	if (!pipeline->pipes)
		return ;
	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		close(pipeline->pipes[i][0]);
		close(pipeline->pipes[i][1]);
		i++;
	}
}

void	setup_pipe_fds(t_mini *pipeline, int cmd_index)
{
	// todo null check for pipeline pointer called from execute_single_cmd?
	if (cmd_index > 0)
		dup2(pipeline->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < pipeline->cmd_count - 1)
		dup2(pipeline->pipes[cmd_index][1], STDOUT_FILENO);
}

int	create_pipes(t_mini *pipeline)
{
	int	i;

	if (pipeline->cmd_count <= 1)
		return (0);
	pipeline->pipes = malloc(sizeof(int *) * (pipeline->cmd_count - 1));
	if (!pipeline->pipes)
		return (1);
	i = 0;
	while (i < pipeline->cmd_count - 1)
	{
		pipeline->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipeline->pipes[i])
			return (1);
		if (pipe(pipeline->pipes[i]) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
		i++;
	}
	return (0);
}

int	backup_fds(t_mini *mini)
{
	mini->saved_stdin = dup(STDIN_FILENO);
	mini->saved_stdout = dup(STDOUT_FILENO);
	if (mini->saved_stdin == -1 || mini->saved_stdout == -1)
	{
		if (mini->saved_stdin != -1)
			close(mini->saved_stdin);
		if (mini->saved_stdout != -1)
			close(mini->saved_stdout);
		return (-1);
	}
	mini->redir_flag = 1;
	return (0);
}

void	free_pipeline_pids(t_mini *pipeline)
{
	if (pipeline->pids)
	{
		free(pipeline->pids);
		pipeline->pids = NULL;
	}
}
