/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:12:41 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/19 11:22:42 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_heredoc_delimiter(int pipefd[2], char *line, char *delimiter)
{
	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	exit(0);
}

int	handle_heredoc_redirection(char *delimiter)
{
	int	pipefd[2];
	char	*line;
	pid_t	pid;

	line = NULL;
	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		handle_heredoc_delimiter(pipefd, line, delimiter);
	close(pipefd[1]);
	waitpid(pid, NULL, 0);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		close(pipefd[0]);
		perror("minishell: dup2");
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
