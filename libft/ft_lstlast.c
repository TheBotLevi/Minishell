/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:40:21 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/31 09:50:22 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*int	main()
{
	t_list	*head = ft_lstnew("Node 1");
	t_list	*node2 = ft_lstnew("Node 5");
	t_list	*node3 = ft_lstnew("Node 9");

	ft_lstadd_back(&head, node2);
	ft_lstadd_back(&head, node3);

	printf("Testing ft_lstlast:\n");
	t_list	*last = ft_lstlast(head);
	if (last)
		printf("Last node: %s\n", (char *)last->content);
	else
		printf("List is empty.\n");
	ft_lstclear(&head, free);
	return (0);
}*/