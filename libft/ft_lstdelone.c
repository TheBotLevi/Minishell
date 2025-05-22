/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:05:39 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/31 09:14:08 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

/*void	del(void *content)
{
	free(content);
}

int	main()
{
	t_list	*node = ft_lstnew(malloc(10));
	ft_lstdelone(node, del);

	printf("Testing ft_lstdelone: Check memory with Valgrind.\n");
	return (0);
}*/