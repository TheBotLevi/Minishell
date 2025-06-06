/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 11:25:44 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/06 11:26:34 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	mini_pwd(void)
{
	char	current_dir[BUFFER_SIZE];

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(current_dir, STDOUT_FILENO);
	return (0);
}
