/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/08 20:46:22 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*void	execute_cmd(char *cmd_str, t_env *env_list)
{
	char	**cmd_args;
	char	**paths;
	char	*exec;
	char	**env_array;

	cmd_args = ft_split(cmd_str, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		if (cmd_args)
			free_array(cmd_args);
		ft_putstr_fd("Error: invalid command\n", 2);
		exit(1);
	}
	paths = get_paths_from_list(env_list);
	exec = find_exec(cmd_args[0], paths);
	if (paths)
		free_array(paths);
	if(!exec)
	{
		free_array(cmd_args);
		ft_putstr_fd("Error: command not found\n", 2);
		exit (127);
	}
	env_array = env_list_to_array(env_list);
	execve(exec, cmd_args, env_array);
	free(exec);
	free_array(cmd_args);
	free_array(env_array);
	perror("Error: execve");
	exit (1);
}*/

int	execute_external_cmd(char **args, t_env *env)
{
	pid_t	pid;
	int	status;
	char	**envp;
	char	**paths;
	char	*exec_path;

	envp = env_list_to_array(env);
	pid = fork();
	if (pid == 0)
	{
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
	char	**args;
	int	status;

	if (!line || !*line)
		return (0);
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
	while (path[i])
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
