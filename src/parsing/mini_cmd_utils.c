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

void	free_cmds(t_command *cmd)
{
    t_command	*tmp;

    if (!cmd)
        return ;
    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->argv)
            free_args(cmd->argv);
        free(cmd);
        cmd = tmp;
    }
    /*while (cmd[i])
    {
        if (cmd[i]->argv)
            free_args(cmd[i]->argv);
        free(cmd[i]);
        i++;
    }*/
}
