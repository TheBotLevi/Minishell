/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:13:41 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/18 16:13:48 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_cmds(t_command *args)
{
    /*
    int	i;

    i = 0;*/

    t_command	*tmp;

    if (!args)
        return ;
    while (args)
    {
        if (args->argv)
            free_args(args->argv);
        tmp = args->next;
        free(args);
        args = tmp;
    }
    /*while (args[i])
    {
        if (args[i]->argv)
            free_args(args[i]->argv);
        free(args[i]);
        i++;
    }*/
}
