/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 09:12:04 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/03 17:27:47 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	has_pipes(char *line)
{
	if (!line)
		return (0);
	return (ft_strchr(line, '|') != NULL);
}

static t_mini	*create_cmd_node(char **args)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	ft_bzero(mini, sizeof(t_mini));
	mini->args = args;
	mini->input_fd = -1;
	mini->output_fd = -1;
	mini->next = NULL;
	return (mini);
}

int	parse_pipeline(char *line, t_mini **pipeline)
{
	char	**pipe_parts;
	t_mini	*mini_list;
	t_mini	*current;
	int		i;

	*pipeline = malloc(sizeof(t_mini));
	if (!*pipeline)
		return (1);
	ft_bzero(*pipeline, sizeof(t_mini));
	pipe_parts = ft_split(line, '|');
	if (!pipe_parts)
		return (1);
	
	mini_list = NULL;
	i = 0;
	while (pipe_parts[i])
	{
		current = create_cmd_node(parse_input(pipe_parts[i]));
		if (!current)
			return (1);
		current->next = mini_list;
		mini_list = current;
		i++;
	}
	(*pipeline)->env_struct = mini_list->env_struct;
	(*pipeline)->commands = mini_list;
	(*pipeline)->cmd_count = i;
//	(*pipeline)->pipes = NULL;
//	(*pipeline)->pids = NULL;
	free_args(pipe_parts);
	return (0);
}


