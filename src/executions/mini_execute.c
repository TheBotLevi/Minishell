/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/13 17:52:31 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_external_cmd(t_mini *mini)
{
	pid_t	pid;
	char	*exec_path;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (execute_redirections(mini, 0) != 0)
			exit(1);
		if (set_environment(mini))
			exit(1);
		exec_path = find_exec(mini->cur_cmd->args[0], mini);
		printf("%s", exec_path);
		if (!exec_path)
			free_args(mini->envp);
		execve(exec_path, mini->cur_cmd->args, mini->envp);
		perror("mariashell: execve");
		free_everything(mini);
		free(exec_path);
		exit(126);
	}
	signal(SIGINT, SIG_IGN);
	return (handle_parent_process(pid, mini->envp));
}

int	process_command(t_mini *mini, char *line)
{
	mini->cmds = parse_line_to_commands(line, mini);
	if (!mini->cmds)
		return (0);
	mini->cur_cmd = mini->cmds;
	if (prepare_heredocs(mini) != 0)
		return (mini->exit_status);
	if (mini->cmd_count > 1)
		mini->exit_status = execute_pipeline(mini);
	else if (mini->cur_cmd->args && mini->cur_cmd->args[0])
	{
		if (is_builtin(mini->cur_cmd->args[0]))
			mini->exit_status = execute_builtin_in_parent(mini);
		else
			mini->exit_status = execute_external_cmd(mini);
	}
	if (mini->pipes)
	{
		help_free_pipelines(mini);
		mini->pipes = NULL;
	}
	free_cmds(mini->cmds);
	mini->cmds = NULL;
	mini->cur_cmd = NULL;
	return (mini->exit_status);
}

// Return values: 1 = OK, 0 = not found, -1 = found but not executable
static int	can_execute(char *cmd)
{
	struct stat	s;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &s) == 0)
		{
			if (S_ISDIR(s.st_mode))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putendl_fd(": Is a directory", 2);
				return (-1);
			}
			if (access(cmd, X_OK) == 0)
				return (1);
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			return (-1);
		}
	}
	return (0);
}

static char	*create_full_path(char *cmd, char **paths, t_mini *mini)
{
	char	*path;
	char	*full_path;
	int		i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (can_execute(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	free_args(paths);
	handle_external_command_not_found(mini);
	return (NULL);
}

char	*find_exec(char *cmd, t_mini *mini)
{
	int		exec_check;
	char	**paths;

	paths = get_paths_from_list(mini->env_struct);
	if (!paths)
		free_args(paths);
	if (ft_strchr(cmd, '/'))
	{
		exec_check = can_execute(cmd);
		if (exec_check == 1)
			return (ft_strdup(cmd));
		if (exec_check == -1)
		{
			free_args(paths);
			free_everything(mini);
			exit(126);
		}
		free_args(paths);
		handle_external_file_not_found(mini);
		return (NULL);
	}
	return (create_full_path(cmd, paths, mini));
}
