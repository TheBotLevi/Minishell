/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/07 13:28:53 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_external_cmd(t_mini *mini)
{
	pid_t	pid;
	int	status;
	char	**envp;
	char	**paths;
	char	*exec_path;

	envp = env_list_to_array(mini->env_struct);
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (execute_redirections(mini) != 0)
				exit(1);
		paths = get_paths_from_list(mini->env_struct);
		exec_path = find_exec(mini->args[0], paths);
		if (paths)
			free_args(paths);
		if (exec_path)
		{
			execve(exec_path, mini->args, envp);
			free(exec_path);
		}
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	else if (pid < 0)
	{
		ft_putendl_fd("minishell: Fork failed", STDERR_FILENO);
		status = 1;
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}
	free_args(envp);
	return (status);
}

int	process_command(char *line, t_mini *mini)
{
	t_mini	*pipeline;
	t_mini	*current;

	if (!line || !*line)
		return (0);
	if (has_pipes(line))
	{
		if (parse_pipeline(line, &pipeline) == 0)
		{
			pipeline->env_struct = mini->env_struct;
			current = pipeline->commands;
			while (current)
			{
				current->env_struct = mini->env_struct;
				current = current->next;
			}
			mini->exit_status = execute_pipeline(pipeline);
			free_pipeline(pipeline);
		}
		else
			mini->exit_status = 1;
		update_exit_status(mini);
	}
	else
	{
		mini->args = parse_input(line);
		if (mini->args && mini->args[0])
		{
			if (is_builtin(mini->args[0]))
				mini->exit_status = handle_builtin(mini);
			else
				mini->exit_status = execute_external_cmd(mini);
		}
		if (mini->args)
		{
			free(mini->args);
			mini->args = NULL;
		}
		update_exit_status(mini);
	}
	return (mini->exit_status);
}

static int	can_execute(char *cmd)
{
	struct stat s;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &s) == 0)
		{
			if (S_ISDIR(s.st_mode))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putendl_fd(": is a directory", 2);
				return (0);
			}
			if (access(cmd, X_OK) == 0)
				return (1);
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			return (0);
		}
	}
	return (0);
}

char	*find_exec(char *cmd, char **paths)
{
	char	*path;
	char	*full_path;
	int	i;

	if (ft_strchr(cmd, '/'))
	{
		if (can_execute(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])  // Fixed: was 'path[i]'
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (can_execute(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
