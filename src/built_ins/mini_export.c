/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:27:49 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/06 17:44:37 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_valid(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	display_exported_vars(t_env *env)
{
	
}

int	mini_export(char **args, t_env **env)
{
	int	i;
	t_env	*env_list;

	i = 1;
	env_list = NULL;
	while (args[i])
	{
		if (ft_strcmp(args[i], env_list->key) == 0)
		i++;
	}
}
