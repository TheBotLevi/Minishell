/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:34:15 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/30 11:22:52 by ljeribha         ###   ########.fr       */
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
	update_env_value(&env_list, "SHELL", "minishell");
	if (getcwd(cwd, BUFFER_SIZE))
	{
		pwd = ft_strjoin("PWD=", cwd);
		if (pwd)
		{
			update_env(&env_list, pwd);
			free(pwd);
		}
	}
	update_env(&env_list, "?=0");
	return (env_list);
}

t_mini	*mini_init(char ** envp)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
	{
		ft_putendl_fd("Error allocating memory for mini", STDERR_FILENO);
		return (NULL);
	}
	mini->env_struct = init_environment(envp);
	if (!mini->env_struct)
	{
		ft_putendl_fd("Error initializing environment", STDERR_FILENO);
		free(mini);
		return (NULL);
	}
	mini->old_path = NULL;
	mini->args = NULL;
	mini->exit_status = 0;
	return (mini);
}
