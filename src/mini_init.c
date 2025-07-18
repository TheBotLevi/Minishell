/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:34:15 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/18 13:01:40 by ljeribha         ###   ########.fr       */
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
//	update_env(&env_list, "?=0");
	return (env_list);
}

static void	mini_init_init(t_mini *mini)
{
	mini->old_path = NULL;
	mini->args = NULL;
	mini->exit_status = 0;
	mini->pipes = NULL;
	mini->cmd_count = 0;
	mini->commands = NULL;
	mini->fd = -1;
	mini->filename = NULL;
	mini->input_fd = -1;
	mini->output_fd = -1;
	mini->pids = NULL;
	mini->pipes = NULL;
	mini->saved_stdin = -1;
	mini->saved_stdout = -1;
	mini->redir_flag = 0;
	mini->original_args = NULL;
}

t_mini	*mini_init(char **envp)
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
	mini_init_init(mini);
	return (mini);
}
