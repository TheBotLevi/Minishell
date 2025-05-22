/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:28:48 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/31 09:40:00 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

/*int	main()
{
	t_list	*head = ft_lstnew("Node 1");
	t_list	*node2 = ft_lstnew("Node 2");
	t_list	*node3 = ft_lstnew("Node 3");

	ft_lstadd_back(&head, node2);
	ft_lstadd_back(&head, node3);

	printf("Testing ft_lstsize:\n");
	int	size = ft_lstsize(head);
	printf("List size: %d\n", size);

	ft_lstclear(&head, free);
	return (0);
}*/