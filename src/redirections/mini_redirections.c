/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:40:46 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/04 12:23:19 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_input_redirection(t_mini *mini)
{
	mini->fd = open(mini->filename, O_RDONLY);
	if (mini->fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (-1);
	}
	if (dup2(mini->fd, STDIN_FILENO) < 0)
	{
		close(mini->fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(mini->fd);
	return (0);
}

static int	handle_output_redirection(t_mini *mini)
{
	mini->fd = open(mini->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (mini->fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (-1);
	}
	if (dup2(mini->fd, STDOUT_FILENO) < 0)
	{
		close(mini->fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(mini->fd);
	return (0);
}

static int	handle_append_redirection(t_mini *mini)
{
	mini->fd = open(mini->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (mini->fd < 0)
	{
		ft_putstr_fd("minishell :", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (-1);
	}
	if (dup2(mini->fd, STDOUT_FILENO) < 0)
	{
		close(mini->fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(mini->fd);
	return (0);
}

int	execute_redirections(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->args[i])
	{
		if (ft_strcmp(mini->args[i], "<") == 0 && mini->args[i + 1])
		{
			mini->filename = mini->args[i + 1];
			if (handle_input_redirection(mini) < 0)
				return (1);
			i += 2;
		}
		else if (ft_strcmp(mini->args[i], ">") == 0 && mini->args[i + 1])
		{
			mini->filename = mini->args[i + 1];
			if (handle_output_redirection(mini) < 0)
				return (2);
			i += 2;
		}
		else if (ft_strcmp(mini->args[i], ">>") == 0 && mini->args[i + 1])
		{
			mini->filename = mini->args[i + 1];
			if (handle_append_redirection(mini) < 0)
				return (3);
			i += 2;
		}
		else if (ft_strcmp(mini->args[i], "<<") == 0 && mini->args[i + 1])
		{
			mini->filename = mini->args[i + 1];
			if (handle_heredoc_redirection(mini, mini->args[i + 1]) < 0)
			{
				restore_main_signals();
				return (4);
			}
			i += 2;
		}
		else
			i++;
	}
	return (0);
}
