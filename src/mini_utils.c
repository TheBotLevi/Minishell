/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:40:14 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/01 16:10:03 by ljeribha         ###   ########.fr       */
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

void	clear_readline_history(void)
{
	rl_clear_history();
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

void	update_exit_status(t_mini *mini)
{
	char	*exit_str;

	exit_str = ft_itoa(mini->exit_status);
	if (exit_str)
	{
//		update_env_value(&mini->env_struct, "?", exit_str);
		free(exit_str);
	}
}

void print_array(char** ar) {
	int i;

	i = 0;
	if (!ar)
		return;
	while (ar[i] != NULL) {
		printf("[%s]", ar[i]);
		i++;
	}
	printf("\n");
}
