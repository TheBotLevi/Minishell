/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handle_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:12:41 by ljeribha          #+#    #+#             */
/*   Updated: 2025/08/13 15:50:02 by ljeribha         ###   ########.fr       */
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

/* checks is variables should be expanded (if delim is not quoted)
 * if any part of the delimiter is unquoted, expand the variables and
 * return the expanded string */
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
	exit(g_exit);
}

static int	process_heredoc(t_mini *mini, t_redirect *redir)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == 0)
		handle_heredoc_delimiter(mini, pipefd, redir);
	signal(SIGINT, SIG_IGN);
	close(pipefd[1]);
	waitpid(pid, &mini->exit_status, 0);
	if (WIFEXITED(mini->exit_status) && WEXITSTATUS(mini->exit_status) == 130)
	{
		close(pipefd[0]);
		redir->fd = -1;
		g_exit = 130;
		return (g_exit);
	}
	redir->fd = pipefd[0];
	return (0);
}

int	prepare_heredocs(t_mini *mini)
{
	t_command	*cmd;
	t_redirect	*redir;
	int			result;

	cmd = mini->cmds;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				result = process_heredoc(mini, redir);
				if (result != 0)
					return (result);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
