/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:52:36 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/28 12:25:43 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	mini_unset(t_mini *mini)
{
	int	i;

	if (!mini->cur_cmd->args[1])
		return (0);
	i = 1;
	mini->exit_status = 0;
	while (mini->cur_cmd->args[i])
	{
		if (!is_valid_export(mini->cur_cmd->args[i])) {
			mini->exit_status = 1;
		}
		remove_env_var(&mini->env_struct, mini->cur_cmd->args[i]);
		i++;
	}
	return (mini->exit_status);
}
