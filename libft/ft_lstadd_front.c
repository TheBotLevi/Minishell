/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:21:34 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/30 17:09:36 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

/*int	main()
{
	t_list	*lst = NULL;
	t_list	*new = malloc(sizeof(t_list));
	if (new)
	{
		new->content = "Hello, World!";
		new->next = NULL;
		ft_lstadd_front(&lst, new);
		printf("Head content: %s\n", (char *)lst->content);
	}
	free(new);
	return (0);
}*/