/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:52:34 by ljeribha          #+#    #+#             */
/*   Updated: 2024/10/31 10:05:56 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	tmp = *lst;
	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

/*int	main()
{
	t_list	*head = ft_lstnew("First");
	t_list	*second = ft_lstnew("Second");
	t_list	*third = ft_lstnew("Third");

	ft_lstadd_back(&head, second);
	ft_lstadd_back(&head, third);

	t_list	*current = head;
	while (current)
	{
		printf("%s -> ", (char *)current->content);
		current = current->next;
	}
	printf("NULL\n");
	t_list	*tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return (0);
}*/