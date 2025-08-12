/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:27:49 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/14 16:12:17 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_no_value_var(char *arg, t_env **env)
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

void	print_exported_var(t_env *env)
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
