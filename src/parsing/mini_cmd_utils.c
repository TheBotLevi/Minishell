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

int get_array_size(char **array)
{
    int size;

    size = 0;
    while (array[size])
        size++;
    return (size);
}

t_token* get_cmd_end (t_token *cmd_start) {
    t_token *current;
    //t_token *cmd_end;

    current = cmd_start;
    //cmd_end = NULL;
    while (current && !current->is_pipe) {
            /*when comment add stop? except heredoc
            if (!parser->current_cmd->is_heredoc && current->is_comment_start){
                //todo do sth -> stop early and end cmd?
                // *end_cmd = current;
                // if (has_redir)
                //		*cmd_start = create_redirs(&cmd, start_redir, end_cmd);
                // return(1);
                break;
            }*/
       // cmd_end = current;
        current = current->next;
    }
    /*if (!current)
        return (cmd_end);*/
    return (current);
}


