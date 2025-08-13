/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:40:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/02 11:13:15 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	handle_empty_filename(t_mini *mini)
{
	if (mini->filename == NULL || mini->filename[0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	print_invalid_export_key_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	is_valid_export(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}
