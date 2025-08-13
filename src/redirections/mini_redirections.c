/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:40:46 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/09 18:12:44 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_empty_filename(t_mini *mini)
{
	if (mini->filename == NULL || mini->filename[0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

static int	handle_input_redirection(t_mini *mini)
{
	if (handle_empty_filename(mini) == -1)
		return (-1);
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
	if (handle_empty_filename(mini) == -1)
		return (-1);
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
	if (handle_empty_filename(mini) == -1)
		return (-1);
	mini->fd = open(mini->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

int	execute_redirections(t_mini *mini)
{
	int			error;
	t_redirect	*redir;

	if (!mini->cur_cmd->args)
		return (0);
	error = 0;
	redir = mini->cur_cmd->redirections;
	while (redir)
	{
		mini->filename = redir->filename;
		if (redir->type == REDIR_INPUT)
			error = handle_input_redirection(mini);
		else if (redir->type == REDIR_OUTPUT)
			error = handle_output_redirection(mini);
		else if (redir->type == REDIR_APPEND)
			error = handle_append_redirection(mini);
		else if (redir->type == REDIR_HEREDOC){
			//error = handle_heredoc_redirection(mini, redir);
			if (dup2(redir->fd, STDIN_FILENO) < 0)
				perror("dup2");
			close(redir->fd);
		}
		if (error)
			return (1);
		redir = redir->next;
	}
	return (0);
}
