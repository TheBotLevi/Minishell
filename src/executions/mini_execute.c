/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/18 17:52:51 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_redirections(t_mini *mini)
{
	int	i;

	i = 0;
	while(mini->args[i])
	{
		if (ft_strcmp(mini->args[i], "<") == 0 || ft_strcmp(mini->args[i], ">") == 0 ||
		ft_strcmp(mini->args[i], "<<") == 0 || ft_strcmp(mini->args[i], ">>") == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	backup_fds(t_mini *mini)
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

static void	restore_fds(t_mini *mini)
{
	if (mini->redir_flag)
	{
		if (mini->saved_stdin != -1)
		{
			dup2(mini->saved_stdin, STDIN_FILENO);
			close(mini->saved_stdin);
		}
		if (mini->saved_stdout != 1)
		{
			dup2(mini->saved_stdout, STDOUT_FILENO);
			close(mini->saved_stdout);
		}
		mini->redir_flag = 0;
	}
}

int	execute_builtin_in_parent(t_mini *mini)
{
	int	res;
	int	has_redir;

	has_redir = check_redirections(mini);
	if (has_redir)
	{
		if (backup_fds(mini) == -1)
			return (1);
		if (execute_redirections(mini) != 0)
		{
			restore_fds(mini);
			return (2);
		}
	}
	res = handle_builtin(mini);
	if (has_redir)
		restore_fds(mini);
	return (res);
}

static int	handle_parent_process(pid_t pid, char **envp)
{
	int	status;

	if (pid < 0)
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
	envp = NULL;
	return (status);
}

static void	handle_external_command(t_mini *mini)
{
	ft_putstr_fd("mariashell: ", STDERR_FILENO);
	ft_putstr_fd(mini->args[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit(127);
}

int	execute_external_cmd(t_mini *mini)
{
	pid_t	pid;
	char	**envp;
	char	**paths;
	char	*exec_path;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (execute_redirections(mini) != 0)
		{
			cleanup_redir(mini);
			free_args(envp);
			exit(1);
		}
		envp = env_list_to_array(mini->env_struct);
		if (!envp)
		{
			cleanup_redir(mini);
			exit(1);
		}
		paths = get_paths_from_list(mini->env_struct);
		exec_path = find_exec(mini->args[0], paths);
		if (paths)
			free_args(paths);
		if (!exec_path)
		{
			cleanup_redir(mini);
			free_args(envp);
			handle_external_command(mini);
		}
		execve(exec_path, mini->args, envp);
		perror("mariashell: execve");
		cleanup_redir(mini);
		free(exec_path);
		free_args(envp);
		exit(126);
	}
	signal(SIGINT, SIG_IGN);
	return (handle_parent_process(pid, envp));
}

static int	handle_single_command(t_mini *mini)
{
	if (is_builtin(mini->args[0]))
		return (execute_builtin_in_parent(mini));
	else
		return (execute_external_cmd(mini));
}

static int	handle_pipeline(t_mini *mini, char *line)
{
	t_mini	*pipeline;
	t_mini	*current;

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
	return (mini->exit_status);
}

int	process_command(char *line, t_mini *mini)
{
	if (!line || !*line)
		return (0);
	if (has_pipes(line))
		return (handle_pipeline(mini, line));
	else
	{
		mini->args = parse_input(line);
		if (mini->args && mini->args[0])
			mini->exit_status = handle_single_command(mini);
		if (mini->args)
		{
			free_args(mini->args);
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
