/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:52:36 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/06 11:27:11 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	mini_unset(char **cmds, t_env **env)
{
	int	i;
	int	status;

	if (!cmds[1])
		return (0);
	i = 1;
	status = 0;
	while (cmds[i])
	{
		if (remove_env_var(env, cmds[i]) == 0)
			status = 1;
		i++;
	}
	return (status);
}
