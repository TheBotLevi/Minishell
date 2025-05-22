/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:44:50 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/30 15:59:02 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

/*int	main()
{
	int	data = 42;
	t_list	*node = ft_lstnew(&data);

	if (node)
		printf("Node content: %d\n", *(int *)node->content);
	else
		printf("Failed to allocate memory for new node.\n");
	free (node);
	return (0);
}*/