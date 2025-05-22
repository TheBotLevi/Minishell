/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:01:50 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/31 10:06:40 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
}

/*void	del(void *content)
{
	free(content);
}

int	main()
{
	t_list	*head = ft_lstnew(malloc(10));
	t_list	*node = ft_lstnew(malloc(10));

	ft_lstadd_back(&head, node);

	ft_lstclear(&head, del);

	printf("Testing ft_lstclear: Check memory with valgrind.\n");
	return (0);
}*/