/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:27:49 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/14 16:12:17 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_no_value_var(char *arg, t_env **env)
{
	char	*key;
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, arg) == 0)
			return (0);
		current = current->next;
	}
	key = ft_strdup(arg);
	if (!key)
		return (1);
	return (add_new_env_var(env, key, NULL));
}

static void	print_exported_var(t_env *env)
{
	ft_putstr_fd("export ", STDOUT_FILENO);
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
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	if (count == 0)
		return ;
	sorted_env = malloc(sizeof(t_env *) * count);
	if (!sorted_env)
		return ;
	fill_sorted_env(env, sorted_env, count);
	free (sorted_env);
}

static int	handle_export_arg(char *arg, t_env **env)
{
	char	*equals_pos;

	if (!is_valid_export(arg))
	{
		ft_putstr_fd("mariashell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		return (handle_no_value_var(arg, env));
	return (update_env(env, arg));
}


int	mini_export(t_mini *mini)
{
	int	i;
	int	status;

	status = 0;
	if (!mini->args[1])
	{
		display_exported_vars(mini->env_struct);
		return (0);
	}
	i = 1;
	while (mini->args[i])
	{
		if (ft_strchr(mini->args[i], '=') == NULL)
		{
			if (handle_no_value_var(mini->args[i], &mini->env_struct))
				status = 1;
		}
		else if (handle_export_arg(mini->args[i], &mini->env_struct))
			status = 1;
		i++;
	}
	return (status);
}
