/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:52:36 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/04 17:07:29 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	only_n(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	mini_echo(char **cmds)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;
	if (cmds[i] == ft_strncmp(cmds[i][0], '-', 2) && only_n(cmds[1]))
	{
		new_line = 0;
		i++;
	}
	while (cmds[i])
	{
		ft_putstr_fd(cmds[i], STDOUT_FILENO);
		if (cmds[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

int	mini_pwd(void)
{
	char	current_dir[BUFFER_SIZE];

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(current_dir, STDOUT_FILENO);
	return (0);
}

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

void	mini_env(t_env **env)
{
	t_env	*env_list;

	env_list = *env;
	while (env_list)
	{
		if (env_list->value)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}
