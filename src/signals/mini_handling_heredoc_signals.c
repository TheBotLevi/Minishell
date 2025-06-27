/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_handling_heredoc_signals.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:06:35 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/25 18:04:00 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_exit = 0;

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_exit = 130;
	//signal(SIGINT, SIG_DFL);
	//kill(0, SIGINT);
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

void	handle_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_main_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
