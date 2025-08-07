/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:00:00 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/16 13:22:53 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/*
static int	create_pipes(t_mini *pipeline)
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
        {
            // Clean up previously allocated pipes
            while (--i >= 0)
            {
                close(pipeline->pipes[i][0]);
                close(pipeline->pipes[i][1]);
                free(pipeline->pipes[i]);
            }
            free(pipeline->pipes);
            pipeline->pipes = NULL;
            return (1);
        }
        if (pipe(pipeline->pipes[i]) == -1)
        {
            perror("minishell: pipe");
            // Clean up on pipe failure
            free(pipeline->pipes[i]);
            while (--i >= 0)
            {
                close(pipeline->pipes[i][0]);
                close(pipeline->pipes[i][1]);
                free(pipeline->pipes[i]);
            }
            free(pipeline->pipes);
            pipeline->pipes = NULL;
            return (1);
        }
        i++;
    }
    return (0);
}
*/

static int	create_pipes(t_mini *pipeline)
{
	int	i;
	t_command	*command;

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

static void	setup_pipe_fds(t_mini *pipeline, int cmd_index)
{
	if (cmd_index > 0)
		dup2(pipeline->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < pipeline->cmd_count - 1)
		dup2(pipeline->pipes[cmd_index][1], STDOUT_FILENO);
}

static void	close_all_pipes(t_mini *pipeline)
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

static void	execute_single_cmd(t_command* cmd, t_mini *pipeline,
							int cmd_index)
{
	char	**envp;
	char	**paths;
	char	*exec_path;

	setup_child_signals();
	setup_pipe_fds(pipeline, cmd_index);
	close_all_pipes(pipeline);
	
	// Handle redirections for this command
	if (execute_redirections(mini) != 0)
		exit(1);
	
	// Handle builtins
	if (is_builtin(cmd->args[0]))
		exit(handle_builtin(mini));

	// Execute external commands
	envp = env_list_to_array(cmd->env_struct);
	paths = get_paths_from_list(cmd->env_struct);
	exec_path = find_exec(cmd->args[0], paths);

	if (paths)
		free_args(paths);
	if (exec_path)
	{
		execve(exec_path, cmd->args, envp);
		perror("minishell: execve");
		free(exec_path);
	}
	else
	{
		ft_putstr_fd("mariashell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	if (envp)
		free_args(envp);
	exit(127);
}

static int	wait_for_processes(t_mini *pipeline)
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
	if (pipeline->pids)
	{
		free(pipeline->pids);
		pipeline->pids = NULL;
	}
	return (last_status);
}

int	create_and_fork_process(t_mini *pipeline)
{
	t_mini	*current;
	t_command	*command;
	int	cmd_index;

	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
		return (1);
	//current = pipeline->commands;
	cmd_index = pipeline->cmd_count - 1;
	while (command)
	{
		pipeline->pids[cmd_index] = fork();
		if (pipeline->pids[cmd_index] == 0)
			execute_single_cmd(current, pipeline, cmd_index);
		else if (pipeline->pids[cmd_index] < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		command = command->next;
		cmd_index--;
	}
	return (0);
}

int	execute_pipeline(t_mini *pipeline)
{
	int		status;

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
