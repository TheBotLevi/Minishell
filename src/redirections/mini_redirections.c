/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:40:46 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/09 13:39:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	**fill_clean_args(t_mini *mini, char **new_args)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (mini->args[i])
	{
		if (ft_strcmp(mini->args[i], "<") == 0 || ft_strcmp(mini->args[i], ">") == 0
		|| ft_strcmp(mini->args[i], ">>") == 0 || ft_strcmp(mini->args[i], "<<") == 0)
			i += 2;
		else
		{
			new_args[j] = ft_strdup(mini->args[i]);
			j++;
			i++;
		}
	}
	new_args[j] = NULL;
	return (new_args);
}

static char	**remove_redirections(t_mini *mini)
{
	char	**new_args;
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (mini->args[i])
	{
		if (ft_strcmp(mini->args[i], "<") == 0 || ft_strcmp(mini->args[i], ">") == 0
		|| ft_strcmp(mini->args[i], ">>") == 0 || ft_strcmp(mini->args[i], "<<") == 0)
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	return (fill_clean_args(mini, new_args));
}

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
//	printf("DEBUG: Opening file '%s' for append\n", mini->filename);
	mini->fd = open(mini->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (mini->fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (-1);
	}
//	printf("DEBUG: File opened successfully, fd = %d\n", mini->fd);
	if (dup2(mini->fd, STDOUT_FILENO) < 0)
	{
		close(mini->fd);
		perror("minishell: dup2");
		return (-1);
	}
//	printf("DEBUG: dup2 successfull, stdout redirected\n");
	close(mini->fd);
	return (0);
}

int	handle_redirections(t_mini *mini, int i)
{
	if (ft_strcmp(mini->args[i], "<") == 0 && mini->args[i + 1])
	{
		mini->filename = mini->args[i + 1];
		if (handle_input_redirection(mini) < 0)
			return (1);
	}
	else if (ft_strcmp(mini->args[i], ">") == 0 && mini->args[i + 1])
	{
		mini->filename = mini->args[i + 1];
		if (handle_output_redirection(mini) < 0)
			return (2);
	}
	else if (ft_strcmp(mini->args[i], ">>") == 0 && mini->args[i + 1])
	{
		mini->filename = mini->args[i + 1];
		if (handle_append_redirection(mini) < 0)
			return (3);
	}
	else if (ft_strcmp(mini->args[i], "<<") == 0 && mini->args[i + 1])
	{
		mini->filename = mini->args[i + 1];
		if (handle_heredoc_redirection(mini, mini->args[i + 1]) < 0)
		{
			restore_main_signals();
			return (4);
		}
	}
	return (0);
}

int	execute_redirections(t_mini *mini)
{
	int	i;
	char	**clean_args;
	int	rt;

	if (!mini->args)
		return (0);
	i = 0;
	while (mini->args[i])
	{
		rt = handle_redirections(mini, i);
		if (rt != 0)
			return (rt);
		if (ft_strcmp(mini->args[i], "<") == 0 || ft_strcmp(mini->args[i], ">") == 0
		|| ft_strcmp(mini->args[i], ">>") == 0 || ft_strcmp(mini->args[i], "<<") == 0)
			i += 2;
		else
			i++;
	}
	clean_args = remove_redirections(mini);
	if (clean_args)
	{
		free_args(mini->args);
		mini->args = clean_args;
	}
	return (0);
}
