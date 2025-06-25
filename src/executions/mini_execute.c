/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/25 17:18:00 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_external_cmd(char **args, t_env *env)
{
	pid_t	pid;
	int	status;
	char	**envp;
	char	**paths;
	char	*exec_path;

	envp = env_list_to_array(env);
	if (execute_redirections(args) != 0)
			exit(1);
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		paths = get_paths_from_list(env);
		exec_path = find_exec(args[0], paths);
		if (paths)
			free_args(paths);
		if (exec_path)
		{
			execve(exec_path, args, envp);
			free(exec_path);
		}
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
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

int	process_command(char *line, t_env **env)
{
	char		**args;
	int			status;
	t_pipeline	*pipeline;

	if (!line || !*line)
		return (0);
	// Check if command contains pipes
	if (has_pipes(line))
	{
		if (parse_pipeline(line, &pipeline) == 0)
		{
			status = execute_pipeline(pipeline, env);
			free_pipeline(pipeline);
		}
		else
			status = 1;
		update_exit_status(env, status);
		return (status);
	}
	// Handle single command (no pipes)
	args = parse_input(line);
	if (!args || !args[0])
	{
		if (args)
			free_args(args);
		return (0);
	}
	if (is_builtin(args[0]))
		status = handle_builtin(args, env);
	else
		status = execute_external_cmd(args, *env);
	update_exit_status(env, status);
	free_args(args);
	return (status);
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
