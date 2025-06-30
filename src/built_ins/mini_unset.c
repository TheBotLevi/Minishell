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

	if (!mini->args[1])
		return (0);
	i = 1;
	mini->exit_status = 0;
	while (mini->args[i])
	{
		if (remove_env_var(&mini->env_struct, mini->args[i]) == 0)
			mini->exit_status = 1;
		i++;
	}
	return (mini->exit_status);
}
