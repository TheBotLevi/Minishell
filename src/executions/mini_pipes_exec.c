/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:00:00 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/09 15:45:52 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	try_run_cmd(t_mini *pipeline, char *exec_path, char **envp)
{
	if (exec_path)
	{
		execve(exec_path, pipeline->cur_cmd->args, envp);
		perror("minishell: execve");
		free(exec_path);
	}
	else
	{
		ft_putstr_fd("mariashell: ", STDERR_FILENO);
		ft_putstr_fd(pipeline->cur_cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
}

/*
 * check to either: Handle redirections
 *  Handle builtins
 * or Execute external commands*/
static void	execute_single_cmd(t_mini *pipeline, int cmd_index)
{
	char	**paths;
	char	*exec_path;

	setup_child_signals();
	setup_pipe_fds(pipeline, cmd_index);
	close_all_pipes(pipeline);
	if (execute_redirections(pipeline) != 0)
	{
		free_everything(pipeline);
		exit(1);
	}
	if (is_builtin(pipeline->cur_cmd->args[0]))
		exit(handle_builtin(pipeline, 0));
	pipeline->envp = env_list_to_array(pipeline->cur_cmd->env_struct);
	paths = get_paths_from_list(pipeline->cur_cmd->env_struct);
	exec_path = find_exec(pipeline->cur_cmd->args[0], pipeline);
	if (paths)
		free_args(paths);
	try_run_cmd(pipeline, exec_path, pipeline->envp);
	free_everything(pipeline);
	exit(127);
}

// detect exit status or signal EPIPE/SIGPIPE resulting in exit 141
static int	wait_for_processes(t_mini *pipeline)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipeline->cmd_count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		i++;
	}
	free_pipeline_pids(pipeline);
	return (status);
}

int	create_and_fork_process(t_mini *pipeline)
{
	t_command	*current;
	int			cmd_index;

	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
		return (1);
	current = pipeline->cmds;
	cmd_index = 0;
	while (current)
	{
		pipeline->cur_cmd = current;
		pipeline->pids[cmd_index] = fork();
		if (pipeline->pids[cmd_index] == 0)
			execute_single_cmd(pipeline, cmd_index);
		else if (pipeline->pids[cmd_index] < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		current = current->next;
		cmd_index++;
	}
	return (0);
}

int	execute_pipeline(t_mini *pipeline)
{
	int	status;

	if (!pipeline || pipeline->cmd_count == 0)
		return (1);
	if (create_pipes(pipeline) != 0)
		return (2);
	if (create_and_fork_process(pipeline) != 0)
		return (3);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close_all_pipes(pipeline);
	status = wait_for_processes(pipeline);
	setup_signals();
	return (status);
}
