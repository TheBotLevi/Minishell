/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/07 16:01:00 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute_cmd(char *cmd_str, t_env *env_list)
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
}

int	execute_external_cmd(char **args, t_env *env)
{
	pid_t	pid;
	int	status;
	t_env	**envp;

	envp = env_list_to_array(env);
	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(args[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			exit(127);
		}
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
