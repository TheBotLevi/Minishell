/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:34:15 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/28 13:27:18 by ljeribha         ###   ########.fr       */
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

void	mini_init(t_mini *mini)
{
	char	**env;

	mini = malloc(sizeof(t_mini));
	if (!mini)
	{
		ft_putendl_fd("Error allocating memory for mini", STDERR_FILENO);
		return (1);
	}
	mini->env_struct = init_environment(env);
	if (!mini->env_struct)
	{
		ft_putendl_fd("Error initializing environment", STDERR_FILENO);
		return (2);
	}
	mini->old_path = NULL;
	mini->args = NULL;
	mini->exit_status = 0;
}
