/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:27:49 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/07 15:15:57 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_exported_var(t_env *env)
{
	ft_putstr_fd("export", STDOUT_FILENO);
	ft_putstr_fd(env->key, STDOUT_FILENO);
	if (env->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putchar_fd('\"', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	fill_sorted_env(t_env *env, t_env **sorted_env, int count)
{
	t_env	*current;
	int	i;

	current = env;
	i = 0;
	while (current && i < count)
	{
		sorted_env[i++] = current;
		current = current->next;
	}
	sort_env_vars(sorted_env, count);
	i = 0;
	while (i < count)
	{
		print_exported_var(sorted_env[i]);
		i++;
	}
}

static void	display_exported_vars(t_env *env)
{
	t_env	**sorted_env;
	t_env	*temp;
	int	count;

	count = 0;
	temp = env;
	while (env)
	{
		count++;
		env = env->next;
	}
	if (count == 0)
		return ;
	sorted_env = malloc(sizeof(t_env *) * count);
	if (!sorted_env)
		return (NULL);
	fill_sorted_env(env, sorted_env, count);
	free (sorted_env);
}

static int	handle_export_arg(char *arg, t_env **env)
{
	if (!is_valid_export(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	return (update_env(env, arg));
}

int	mini_export(char **args, t_env **env)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
	{
		display_exported_vars(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(args[i], *env))
			status = 1;
		i++;
	}
	return (status);
}
