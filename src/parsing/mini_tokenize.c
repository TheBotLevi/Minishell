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

#include "../../inc/minishell.h"

int is_special_char(const char c) {
    return(is_in_set(c, "$#'~\\|\"")); //todo is - option necesary?
}
// todo differentiate between in-string special chars, operators etc..?

//int is_special_char_in_str(const char c) {}

int is_whitespace(const char c) {
    return(is_in_set(c, " \t\n\r"));
}

// split on IFS
char* get_ifs_from_env(t_mini *mini) {
    static char default_ifs[4];
    //t_env *current;
    default_ifs[0] = ' ';
    default_ifs[1] = '\t';
    default_ifs[2] = '\n';
    default_ifs[3] = '\0';
    if (!mini || !mini->env_struct)
        return (default_ifs);
    if (get_env_value(mini->env_struct, "IFS"))
        return (get_env_value(mini->env_struct, "IFS"));
    return (default_ifs);
    /*current = mini->env_struct;
    while (current) {
        if (ft_strcmp(current->key, "IFS") == 0) {
            if (!current->value)
                return (default_ifs);
            return (current->value);
        }
        current = current->next;
    }
    return (default_ifs);*/
}

//char** split_on_str

typedef struct s_token_flags {
    int in_comment;
    int in_single_quote;
    int in_double_quote;
    int in_string;
    int in_special;
    int in_var_expansion;
    int is_pipe;
    int is_lt_redir;
    int is_gt_redir;
    int in_append_redir;
    int in_heredoc_redir;

}  t_token_flags;

void set_flags(t_token_flags flags, char *str) {

    // special chars

    // " for double quotes
    if (*str == '"' && !flags.in_single_quote && !flags.in_double_quote)
        flags.in_double_quote = 1;
    // ' for single quotes
    if (*str == '\'' && !flags.in_double_quote && !flags.in_single_quote)
        flags.in_single_quote = 1;
    // # for comment
    if (is_whitespace(*str) && *str+1 && *str+1 == '#'
        && !flags.in_double_quote && !flags.in_single_quote)
        flags.in_comment = 1;
    //$ for variables in plain args or in double quotes
    if (*str == '$' && !flags.in_single_quote)
        flags.in_var_expansion = 1;
    // | for pipe
    if (*str == '|' && !flags.in_single_quote
        && !flags.in_double_quote && !flags.in_comment)
        flags.is_pipe = 1;
    // < for redirection
    if (*str == '<' && !flags.in_single_quote
        && !flags.in_double_quote && !flags.in_comment)
        flags.is_lt_redir = 1;
    // > for redirection
    if (*str == '>' && !flags.in_single_quote
        && !flags.in_double_quote && !flags.in_comment)
        flags.is_gt_redir = 1;
    // >> for append
    if (*str == '>' && *str+1 && *str+1 == '>'
        && !flags.in_single_quote && !flags.in_double_quote && !flags.in_comment)
        flags.in_append_redir = 1;
    // << for heredoc
    if (*str == '<' && *str+1 && *str+1 == '<'
        && !flags.in_single_quote && !flags.in_double_quote && !flags.in_comment)
        flags.in_heredoc_redir = 1;

    // unset flags
    /*
    // # for comment
    if (is_whitespace(*str) && *str+1 == '#' && !flags.in_double_quote && !flags.in_single_quote)
        flags.in_comment = 1;
    //$ for variables in plain args or in double quotes
    if (*str == '$' && !flags.in_single_quote)
        flags.in_var_expansion = 1;*/
    // " for double quotes
    if (*str == '"' && flags.in_double_quote)
        flags.in_double_quote = 0;
    // for single quotes
    if (*str == '\'' && flags.in_single_quote)
        flags.in_single_quote = 0;
}

void print_array(char** ar) {

    while (ar && *ar) {
        printf("%s\n", *ar);
        ar++;
    }
}
char** split_line(char *line) {
    char **tokens;
    //char *token;
    t_token_flags *flags;

    tokens = NULL;
    ft_memset(tokens, 0, sizeof(char*));
    tokens = ft_split_on_str(line, get_ifs_from_env(NULL));
    print_array(tokens);
    flags = NULL;
    ft_memset(flags, 0, sizeof(t_token_flags));
    while (*line) {
        set_flags(*flags, line);
    }
    return (tokens);
}




int	process_command2(char *line, t_mini *mini)
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


