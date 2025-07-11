/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:07:11 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/11 13:07:21 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// todo mark and cmd sep: |, <, >, <<, >> () to split into commands


//todo add test case: > cannot be the last symbol in a valid redirection in Bash.

/* Handle delimiter, can be quoted, unquoted or mix

If onlz whitespace(IFS) and then comment (e.g. cat <<    # comment),
comemnt will be seen as commernt, otherwise parsed as delim str

If any part of the delimiter is unquoted, then:
    Variable expansion, command substitution, and backslash
     interpretation will happen in the here-document body.
Bash treats the first word after << (up to the next unquoted whitespace) as the delimiter —
it can include quotes only if there's no unquoted space between parts.
 */
void parse_delimiter(t_token *current, t_token *next) {
    /*int i;
    int is_delim_quoted;
    int is_delim_unquoted;
    int is_delim_mixed;
    int is_delim_empty;
    int is_delim_whitespace;
    int is_delim_comment;
    int is_delim_start;
    int is_delim_end;

    is_delim_quoted = 0;
    is_delim_unquoted = 0;
    is_delim_mixed = 0;
    is_delim_empty = 0;
    is_delim_whitespace = 0;
    is_delim_comment = 0;
    is_delim_start = 0;
    is_delim_end = 0;*/
    (void)current;
    while (next)
        next = next->next;

    //todo look for quoted argument
    //next->is_comment = 0; //todo deal with this in syntax parsing
    //next->is_comment_start = 0;
}

// todo perform variable substitution

int	process_command2(char *line, t_mini *mini)
{
    t_mini	*pipeline;

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
