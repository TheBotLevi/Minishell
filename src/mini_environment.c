/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:05:09 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/18 11:18:56 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	insert_env_var(t_env **env_list, t_env *new_node, char *env_key)
{
	char	*equals_pos;

	equals_pos = ft_strchr(env_key, '=');
	if (equals_pos)
	{
		new_node->key = ft_substr(env_key, 0, equals_pos - env_key);
		new_node->value = ft_strdup(equals_pos + 1);
		if (!new_node->key || !new_node->value)
		{
			if (new_node->key)
				free(new_node->key);
			if (new_node->value)
				free(new_node->value);
			free(new_node);
			free_env_list(*env_list);
			return (1);
		}
		new_node->next = *env_list;
		*env_list = new_node;
	}
	else
		free(new_node);
	return (0);
}

t_env	*create_env_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
		{
			free_env_list(env_list);
			return (NULL);
		}
		ft_bzero(new_node, sizeof(t_env));
		if (insert_env_var(&env_list, new_node, env[i]))
			return (NULL);
		i++;
	}
	return (env_list);
}

char	**get_paths_from_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "PATH") == 0)
			return (ft_split(current->value, ':'));
		current = current->next;
	}
	return (NULL);
}

int	fill_env_list(char **env_array, t_env *env_list, int *i)
{
	char	*tmp;

	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			tmp = ft_strjoin(env_list->key, "=");
			if (!tmp)
				return (1);
			env_array[*i] = ft_strjoin(tmp, env_list->value);
			free(tmp);
			if (!env_array[*i])
				return (1);
			(*i)++;
		}
		env_list = env_list->next;
	}
	return (0);
}

char	**env_list_to_array(t_env *env_list)
{
	int		count;
	int		i;
	char	**env_array;

	count = ft_envsize(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	if (fill_env_list(env_array, env_list, &i))
	{
		free_env_array(env_array, i);
		return (NULL);
	}
	env_array[i] = NULL;
	return (env_array);
}
