/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:02:31 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/09 17:46:26 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	restore_fds(t_mini *mini)
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

	has_redir = mini->cur_cmd->has_redir;
	if (has_redir)
	{
		if (backup_fds(mini) == -1)
			return (1);
		if (execute_redirections(mini) != 0)
		{
			restore_fds(mini);
			return (1);
		}
	}
	res = handle_builtin(mini);
	if (has_redir)
		restore_fds(mini);
	return (res);
}

int	handle_parent_process(pid_t pid, char **envp)
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
		if (status == 2)
			write(STDERR_FILENO, "\n", 1);
	}
	free_args(envp);
	envp = NULL;
	return (status);
}

void	handle_external_command_not_found(t_mini *mini)
{
	ft_putstr_fd("mariashell: ", STDERR_FILENO);
	ft_putstr_fd(mini->cur_cmd->args[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit(127);
}

void	handle_external_file_not_found(t_mini *mini)
{
	ft_putstr_fd("mariashell: ", STDERR_FILENO);
	ft_putstr_fd(mini->cur_cmd->args[0], STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	exit(127);
}
