/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:34:15 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/04 16:53:21 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*init_environment(char **env)
{
	t_env	*env_list;
	char	cwd[BUFFER_SIZE];
	char	*pwd;

	env_list = create_env_list(env);
	if (!env_list)
		return (NULL);
	update_env(&env_list, "SHELL=minishell");
	if (getcwd(cwd, BUFFER_SIZE))
	{
		ft_split("PWD=", cwd);
		update_env(&env_list, pwd);
		free(pwd);
	}
	update_env(&env_list, "?=0");
	return (env_list);
}
