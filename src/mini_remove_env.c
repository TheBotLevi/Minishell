/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_remove_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:12:04 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/03 13:48:56 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	remove_env_head(t_env **env, char *key)
{
	t_env *current;

	if (!env || !*env || !key)
		return (0);
	if (ft_strcmp((*env)->key, key) == 0)
	{
		current = *env;
		*env = current->next;
		free(current->key);
		free(current->value);
		free(current);
		return (1);
	}
	return (0);
}

int	remove_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return (0);
	if (remove_env_head(env, key))
		return (1);
	prev = *env;
	current = (*env)->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
