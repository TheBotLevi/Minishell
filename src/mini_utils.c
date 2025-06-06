/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:40:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/06 16:10:25 by ljeribha         ###   ########.fr       */
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

void	surpress_rl_leaks(void)
{
	rl_clear_history();
	rl_readline_name = "Minishell";
}

/*void	cmd_pwd(void)
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE) == NULL)
	{
		printf("Error: cannot get current working directory path!\n");
		return ;
	}
	printf("%s\n", buffer);
}
*/

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

char	**parse_input(char *line)
{
	return (ft_split(line, ' '));
}

void	sort_env_vars(t_env **sorted_env, int count)
{
	int	i;
	int	j;
	t_env	*temp;

	i = 0;
	j = 0;
	temp = NULL;
	while (i < count - 1)
	{
		while (j < count - i - 1)
		{
			if (ft_strcmp(sorted_env[j]->key, sorted_env[j + 1]->key))
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[i + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
