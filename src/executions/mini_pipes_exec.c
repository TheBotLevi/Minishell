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
	//todo null check for pipeline pointer called from execute_single_cmd
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

static void	execute_single_cmd(t_mini *pipeline,
							int cmd_index)
{
	char	**envp;
	char	**paths;
	char	*exec_path;

	setup_child_signals();
	setup_pipe_fds(pipeline, cmd_index);
	close_all_pipes(pipeline);
	
	// Handle redirections for this command
	if (execute_redirections(pipeline) != 0)
		exit(1);
	
	// Handle builtins
	if (is_builtin(pipeline->cur_cmd->args[0]))
		exit(handle_builtin(pipeline));

	// Execute external commands
	envp = env_list_to_array(pipeline->cur_cmd->env_struct);
	paths = get_paths_from_list(pipeline->cur_cmd->env_struct);
	exec_path = find_exec(pipeline->cur_cmd->args[0], paths, pipeline);
	if (paths)
		free_args(paths);
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
	if (envp)
		free_args(envp);
	exit(127);
}

// detect exit status or signal EPIPE/SIGPIPE resulting in exit 141
static int	wait_for_processes(t_mini *pipeline)
{
	int	i;
	int	status;
	int	last_status;
	int signal;

	last_status = 0;
	i = 0;
	while (i < pipeline->cmd_count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			signal = WTERMSIG(status);
			if (signal == SIGPIPE)
			{
				ft_putendl_fd("minishell: Broken pipe", STDERR_FILENO);
				last_status = 141;
			}
			else
				last_status = 128 + signal;
		}
		else if (WIFEXITED(status))
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
	t_command	*current;
	int	cmd_index;

	pipeline->pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipeline->pids)
		return (1);
	//current = pipeline->commands;
	current = pipeline->cmds;
	//cmd_index = pipeline->cmd_count - 1;
	cmd_index = 0;
	while (current)
	{
		pipeline->cur_cmd = current;
		pipeline->pids[cmd_index] = fork();
		if (pipeline->pids[cmd_index] == 0) // --> entering child, pipeline is a copy
			execute_single_cmd(pipeline, cmd_index);
		else if (pipeline->pids[cmd_index] < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		current = current->next;
		//cmd_index--;
		cmd_index++;
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
