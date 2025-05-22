/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 11:05:29 by ljeribha          #+#    #+#             */
/*   Updated: 2025/03/31 08:58:32 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	get_result(char **stash, char **result)
{
	char	*c;
	size_t	len;
	size_t	i;

	c = ft_strchr(*stash, '\n');
	if (c == 0)
		len = ft_strlen(*stash) + 1;
	else
		len = c - *stash + 2;
	*result = (char *)malloc(len * sizeof(char));
	if (!*result)
		return ;
	i = 0;
	while (i < len - 1)
	{
		(*result)[i] = (*stash)[i];
		i++;
	}
	(*result)[i] = '\0';
}

void	remove_result(char **stash)
{
	char	*c;
	char	*temp;

	c = ft_strchr(*stash, '\n');
	if (c == 0)
	{
		free_stash(stash);
		return ;
	}
	temp = ft_strdup(c + 1);
	if (temp == 0)
	{
		free_stash(stash);
		return ;
	}
	free(*stash);
	*stash = temp;
}

static char	*init_and_validate(int fd, char **stash)
{
	char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free_stash(stash);
		return (NULL);
	}
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (*stash == NULL)
		*stash = ft_strdup("");
	return (buffer);
}

static char	*process_line(int rd, char **stash)
{
	char	*line;

	if (rd == -1 || (*stash && ft_strlen(*stash) == 0))
	{
		free_stash(stash);
		return (NULL);
	}
	get_result(stash, &line);
	remove_result(stash);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	int			rd;

	buffer = init_and_validate(fd, &stash);
	if (!buffer)
		return (NULL);
	rd = 1;
	while (rd > 0 && ft_strchr(stash, '\n') == NULL)
		rd = read_buffer(fd, &stash, buffer);
	free(buffer);
	return (process_line(rd, &stash));
}
