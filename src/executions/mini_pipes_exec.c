/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:00:00 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/25 17:43:16 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	create_pipes(t_pipeline *pipeline)
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

static void	setup_pipe_fds(t_pipeline *pipeline, int cmd_index)
{
	if (cmd_index > 0)
		dup2(pipeline->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < pipeline->cmd_count - 1)
		dup2(pipeline->pipes[cmd_index][1], STDOUT_FILENO);
}

static void	close_all_pipes(t_pipeline *pipeline)
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

static void	execute_single_cmd(t_cmd *cmd, t_pipeline *pipeline, 
							int cmd_index, t_env **env)
{
	char	**envp;
	char	**paths;
	char	*exec_path;

	setup_child_signals();
	setup_pipe_fds(pipeline, cmd_index);
	close_all_pipes(pipeline);
	if (is_builtin(cmd->args[0]))
		exit(handle_builtin(cmd->args, env));
	envp = env_list_to_array(*env);
	paths = get_paths_from_list(*env);
	exec_path = find_exec(cmd->args[0], paths);
	if (paths)
		free_args(paths);
	if (exec_path)
	{
		execve(exec_path, cmd->args, envp);
		free(exec_path);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	if (envp)
		free_args(envp);
	exit(127);
}

static int	wait_for_processes(t_pipeline *pipeline)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		i++;
	}
	return (last_status);
}

int	create_and_fork_process(t_pipeline *pipeline, t_env **env)
{
	t_cmd	*current;
	int	cmd_index;

	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
		return (1);
	current = pipeline->commands;
	cmd_index = pipeline->cmd_count - 1;
	while (current)
	{
		pipeline->pids[cmd_index] = fork();
		if (pipeline->pids[cmd_index] == 0)
			execute_single_cmd(current, pipeline, cmd_index, env);
		else if (pipeline->pids[cmd_index] < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		current = current->next;
		cmd_index--;
	}
	return (0);
}

int	execute_pipeline(t_pipeline *pipeline, t_env **env)
{
	int		status;

	if (!pipeline || pipeline->cmd_count == 0)
		return (1);
	if (create_pipes(pipeline) != 0)
		return (2);
	if (create_and_fork_process(pipeline, env) != 0)
		return (3);
	close_all_pipes(pipeline);
	status = wait_for_processes(pipeline);
	return (status);
}
