/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handling_heredoc_signals.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:06:35 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/10 14:04:51 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_exit = 130;
//	signal(SIGINT, SIG_DFL);
//	kill(0, SIGINT);
	write(STDOUT_FILENO, "\n", 1);
//	exit(130);
//	rl_replace_line("", 0);
	rl_done = 1;
	close(STDIN_FILENO);
}

/*
void	handle_heredoc_sigint_child(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_heredoc_signal_child(void)
{
	signal(SIGINT, handle_heredoc_sigint_child);
	signal(SIGQUIT, SIG_IGN);
}
*/

void	handle_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_main_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
//	write(STDOUT_FILENO, "DEBUG: Signals restored\n", 30);
}
