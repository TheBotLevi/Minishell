/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_update_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:00:11 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/03 11:10:42 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	parse_var(char *var_str, char **key, char ** value)
{
	char	*equals_pos;

	equals_pos = ft_strchr(var_str, '=');
	if (!equals_pos)
		return (1);
	*key = ft_substr(var_str, 0, equals_pos - var_str);
	*value = ft_strdup(equals_pos + 1);
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (1);
	}
	return (0);
}

int	update_existing_var(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = value;
			free(key);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	add_new_env_var(t_env **env, char *key, char *value)
{
	t_env *new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		return (1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = *env;
	*env = new_node;
	return (0);
}

int	update_env(t_env **env, char *var_str)
{
	char	*key;
	char	*value;

	if (!env || !var_str)
		return (1);
	if (parse_var(var_str, &key, &value))
		return (1);
	if (update_existing_var(*env, key, value))
		return (0);
	return (add_new_env_var(env, key, value));
}
