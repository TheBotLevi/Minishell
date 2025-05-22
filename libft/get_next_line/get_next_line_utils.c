/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 11:26:13 by ljeribha          #+#    #+#             */
/*   Updated: 2025/03/31 09:07:11 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_stash(char **stash)
{
	if (*stash)
	{
		free(*stash);
		*stash = NULL;
	}
}

int	read_buffer(int fd, char **stash, char *buffer)
{
	char	*temp;
	int		rd;

	ft_bzero(buffer, BUFFER_SIZE + 1);
	rd = read(fd, buffer, BUFFER_SIZE);
	if (rd < 0)
	{
		free_stash(stash);
		return (-1);
	}
	if (rd == 0)
		return (0);
	temp = ft_strjoin(*stash, buffer);
	if (temp == NULL)
	{
		free_stash(stash);
		return (-1);
	}
	free(*stash);
	*stash = temp;
	return (rd);
}
