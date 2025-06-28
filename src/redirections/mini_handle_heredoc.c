/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:12:41 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/28 08:57:14 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/*
static	int	check_exit(t_cmd *cmd, char *line)
{
	if (g_exit == 130)
	{
		cmd->exit_status = 130;
		return (1);
	}
	if (line == NULL)
		return (1);
	return (0);
}*/

static int	if_child_interrupted(t_mini *mini, int pipefd_read)
{
	if (WIFEXITED(mini->exit_status) && WEXITSTATUS(mini->exit_status) == 130)
	{
		close(pipefd_read);
		return (1);
	}
	return (0);
}

static int	handle_heredoc_delimiter(int pipefd[2], char *delimiter)
{
	char	*line;

	close(pipefd[0]);
	handle_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (g_exit == 130)
		{
			free(line);
			close(pipefd[1]);
			printf("exiting handle_heredoc_delimiter\n");
			return(130);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	return(0);
}

int	handle_heredoc_redirection(t_mini *mini, char *delimiter)
{
	int	pipefd[2];
	pid_t	pid;

	mini->exit_status = 0;
	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		handle_heredoc_delimiter(pipefd, delimiter);
	signal(SIGINT, SIG_IGN);
	close(pipefd[1]);
	waitpid(pid, &mini->exit_status, 0);
	if (if_child_interrupted(mini, pipefd[0]))
		return (-1);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		close(pipefd[0]);
		perror("minishell: dup2");
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
