/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execute_redirs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:11:04 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/13 17:11:06 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_redir_error(t_mini *mini, int error, int in_parent)
{
	if (error)
	{
		if (!in_parent)
			free_everything(mini);
		return (1);
	}
	return (0);
}
