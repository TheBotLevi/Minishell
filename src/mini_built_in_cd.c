/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:08:25 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/03 10:09:20 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	mini_cd(char **cmds, t_env **env)
{
	char	*path;
	char	current_dir[BUFFER_SIZE];
	char	*old_path;

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
		return (1);
}
