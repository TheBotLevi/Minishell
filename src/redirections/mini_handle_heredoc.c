/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:12:41 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/10 13:59:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*expand_variables(t_mini *mini, t_redirect *redir, char *line);

void	print_error_and_free_tokens(t_token *tokens)
{
	ft_putendl_fd("mariashell: memory allocation error during "
		"variable expansion", STDERR_FILENO);
	free_tokens(tokens);
	tokens = NULL;
}

/* checks is variables should be expandedn (if delim is not quoted)
 * if any part of the delimiter ois unquoted, expand the variables and
 * returm the expanded string */
static char	*expand_variables(t_mini *mini, t_redirect *redir, char *line)
{
	char	*expanded;
	t_token	*tokens;

	if (!line)
		return (ft_strdup(""));
	if (redir->is_quoted == 1)
	{
		expanded = ft_strdup(line);
		free(line);
		return (expanded);
	}
	tokens = NULL;
	if (create_basic_tokens(line, &tokens))
		return (ft_strdup(""));
	free(line);
	if (expand_vars(mini->exit_status, mini->env_struct, &tokens))
	{
		print_error_and_free_tokens(tokens);
		return (ft_strdup(""));
	}
	expanded = get_char_from_tokens(tokens, get_last_token(tokens));
	free_tokens(tokens);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}

static int	handle_heredoc_delimiter(t_mini *mini, int pipefd[2],
		t_redirect *redir)
{
	char	*line;

	close(pipefd[0]);
	handle_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		check_exit(mini, line, pipefd[1]);
		if (!line || ft_strcmp(line, redir->filename) == 0)
		{
			if (!line)
				ft_putendl_fd("mariashell: warning: here-document delimited "
					"by end-of-file", STDERR_FILENO);
			else
				free(line);
			break ;
		}
		line = expand_variables(mini, redir, line);
		ft_putstr_fd(line, pipefd[1]);
		ft_putchar_fd('\n', pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	free_everything(mini);
	exit(0);
}

int prepare_heredocs(t_mini *mini)
{
	t_command   *cmd;
	t_redirect  *redir;
	int         pipefd[2];
	pid_t       pid;

	cmd = mini->cmds;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC) // your enum/type check
			{
				if (pipe(pipefd) < 0)
					return (perror("pipe"), -1);
				pid = fork();
				if (pid == 0)
					handle_heredoc_delimiter(mini, pipefd, redir);
				close(pipefd[1]);
				waitpid(pid, &mini->exit_status, 0);
				if (WIFEXITED(mini->exit_status) &&
					WEXITSTATUS(mini->exit_status) == 130)
				{
					close(pipefd[0]);
					redir->fd = -1;
					g_exit = 130;
					return (130);
				}
				redir->fd = pipefd[0];
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}


/*
int	finish_process(t_mini *mini, int saved_g_exit, int pipefd[2], pid_t pid)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[1]);
	waitpid(pid, &mini->exit_status, 0);
	restore_main_signals();
	if (WIFEXITED(mini->exit_status) && WEXITSTATUS(mini->exit_status) == 130)
	{
		close(pipefd[0]);
		g_exit = 130;
		return (-1);
	}
	g_exit = saved_g_exit;
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		close(pipefd[0]);
		perror("minishell: dup2");
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

int	handle_heredoc_redirection(t_mini *mini, t_redirect *redir)
{
	int		pipefd[2];
	pid_t	pid;
	int		saved_g_exit;

	saved_g_exit = g_exit;
	g_exit = 0;
	mini->exit_status = 0;
	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		handle_heredoc_delimiter(mini, pipefd, redir);
	else if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		perror("minishell: fork");
		g_exit = saved_g_exit;
		return (-1);
	}
	return (finish_process(mini, saved_g_exit, pipefd, pid));
}*/


