/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:33:52 by mdiederi          #+#    #+#             */
/*   Updated: 2025/08/12 15:33:55 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// had to create function because ft_lstsize in libft has t_list and we need
// to pass a t_env
int	ft_envsize(t_env *lst)
{
	int		count;
	t_env	*current;

	count = 0;
	current = lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	sort_env_vars(t_env **sorted_env, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	temp = NULL;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(sorted_env[j]->key, sorted_env[j + 1]->key) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
