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

t_env	*create_env_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	int	i;
	char	*equals_pos;

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
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos)
		{
			new_node->key = ft_substr(env[i], 0, equals_pos - env[i]);
			new_node->value = ft_strdup(equals_pos + 1);
			if (!new_node->key || !new_node->value)
			{
				if (new_node->key)
					free(new_node->key);
				if (new_node->value)
					free(new_node->value);
				free(new_node);
				free_env_list(env_list);
				return (NULL);
			}
			new_node->next = env_list;
			env_list = new_node;
		}
		else
			free(new_node);
		i++;
	}
	/*
	t_env *tmp = env_list;
	while (tmp)
	{
   		printf("key=%s value=%s\n", tmp->key, tmp->value);
    		tmp = tmp->next;
	}*/
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

//had to create function because ft_lstsize in libft has t_list and we need to pass a t_env
int	ft_envsize(t_env *lst)
{
	int	count;
	t_env *current;

	count = 0;
	current = lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**env_list_to_array(t_env *env_list)
{
	int	count;
	int	i;
	char	*tmp;
	char	**env_array;
	t_env	*current;

	env_array = NULL;
	count = ft_envsize(env_list); //changed to ft_lstsize like you wanted ;)
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		if (!current->key || !current->value)
		{
			current = current->next;
			continue ;
		}
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!env_array[i])
		{
			free_env_array(env_array, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
