/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:22:08 by mdiederi          #+#    #+#             */
/*   Updated: 2025/06/25 11:22:13 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int is_special_char(const char c) {
    return(is_in_set(c, '$#"~\\|\'')); //todo is - option necesary?
}
// todo differentiate between in-string special chars, operators etc..?

// split on IFS


int	process_command(char *line, t_mini *mini)
{
    t_pipeline	*pipeline;

    if (!line || !*line)
        return (0);
    // Check if command contains pipes
    if (has_pipes(line))
    {
        if (parse_pipeline(line, &pipeline) == 0)
        {
            mini->exit_status = execute_pipeline(pipeline);
            free_pipeline(pipeline);
        }
        else
            mini->exit_status = 1;
        update_exit_status(mini);
        return (mini->exit_status);
    }
    // Handle single command (no pipes)
    mini->args = parse_input(line);
    if (!mini->args || !mini->args[0])
    {
        if (mini->args)
            free_args(mini->args);
        return (0);
    }
    if (is_builtin(mini->args[0]))
        mini->exit_status = handle_builtin(mini);
    else
        mini->exit_status = execute_external_cmd(mini);
    update_exit_status(mini);
    free_args(mini->args);
    return (mini->exit_status);
}


