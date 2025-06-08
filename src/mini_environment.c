/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:05:09 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/08 20:48:50 by ljeribha         ###   ########.fr       */
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
			return (NULL);
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos)
		{
			new_node->key = ft_substr(env[i], 0, equals_pos - env[i]);
			new_node->value = ft_strdup(equals_pos + 1);
			new_node->next = env_list;
			env_list = new_node;
		}
		else
		{
			free(new_node);
		}
		i++;
	}
	return (env_list);
}

/*static char	*find_exec(char *cmd_name, char **paths)
{
	int	i;
	char	*tmp;
	char	*exec;

	if (!paths)
		return (NULL);
	if (cmd_name[0] == '/' || cmd_name[0] == '.')
		return (ft_strdup(cmd_name));
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(exec, X_OK) == 0)
			return (exec);
		free(exec);
		i++;
	}
	return (NULL);
}*/

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

char	**env_list_to_array(t_env *env_list)
{
	int	count;
	int	i;
	char	*tmp;
	char	**env_array;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
