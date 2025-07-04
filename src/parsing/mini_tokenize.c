/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:22:08 by mdiederi          #+#    #+#             */
/*   Updated: 2025/07/03 12:41:17 by ljeribha         ###   ########.fr       */
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

void unset_flags(t_token_flags *flags, char c, int quote_flag_set) {
    if (quote_flag_set)
        return ;
    if (c == '"' && flags->in_double_quote)
        flags->in_double_quote = 0;
    if (c == '\'' && flags->in_single_quote)
        flags->in_single_quote = 0;
}

void set_flags(t_token_flags *flags, char **tokens) {

    char c;
    char next_c;
    int quote_flag_set;

    if (!tokens || !tokens[0])
        return ;
    c = *tokens[0];
    next_c = 0;
    if (tokens[1])
        next_c = *tokens[1];
    quote_flag_set = 0;
    // " for double quotes
    if (c == '"' && !flags->in_single_quote && !flags->in_double_quote) {
        flags->in_double_quote = 1;
        quote_flag_set = 1;
    }
    // ' for single quotes
    if (c == '\'' && !flags->in_double_quote && !flags->in_single_quote){
        flags->in_single_quote = 1;
        quote_flag_set = 1;
    }
    // # for comment
    if (is_whitespace(c) && next_c && next_c == '#'
        && !flags->in_double_quote && !flags->in_single_quote)
        flags->in_comment = 1;
    //$ for variables in plain args or in double quotes
    if (c == '$' && !flags->in_single_quote)
        flags->in_var_expansion = 1;
    // | for pipe
    if (c == '|' && !flags->in_single_quote
        && !flags->in_double_quote && !flags->in_comment)
        flags->is_pipe = 1;
    // < for redirection
    if (c == '<' && !flags->in_single_quote
        && !flags->in_double_quote && !flags->in_comment)
        flags->is_lt_redir = 1;
    // > for redirection
    if (c == '>' && !flags->in_single_quote
        && !flags->in_double_quote && !flags->in_comment)
        flags->is_gt_redir = 1;
    // >> for append
    if (c == '>' && next_c && next_c == '>'
        && !flags->in_single_quote && !flags->in_double_quote && !flags->in_comment)
        flags->in_append_redir = 1;
    // << for heredoc
    if (c == '<' && next_c && next_c == '<'
        && !flags->in_single_quote && !flags->in_double_quote && !flags->in_comment)
        flags->in_heredoc_redir = 1;
    unset_flags(flags, c, quote_flag_set);
}
/*
// Helper function to get array size
static int get_array_size(char **array) {
    int size;
    for (size = 0; array[size] != NULL; size++);
    return size;
}

char ***split_further(char **original_array, char *delimiter) {
    char ***result;
    int i;
    int array_size;

    // Count how many strings are in original_array
    for (array_size = 0; original_array[array_size] != NULL; array_size++);

    // Allocate memory for array of char**
    result = (char ***)malloc((array_size + 1) * sizeof(char **));
    if (!result)
        return NULL;

    // For each string in original array
    for (i = 0; original_array[i] != NULL; i++) {
        // Split each string and store the result in result[i]
        result[i] = ft_split_on_str(original_array[i], delimiter);
        if (!result[i]) {
            // Handle error: free previously allocated memory
            while (--i >= 0) {
                free_n_array(result[i], get_array_size(result[i]));
            }
            free(result);
            return NULL;
        }
    }
    result[i] = NULL; // Null terminate the array

    return result;
}
*/


char** split_line(char *line, t_mini *mini) {
    char **arr_quotes_string;
    //char **tokens_head;
    //char **tokens;
    //t_token_flags flags;

    if (!line || !mini)
        return NULL;
    arr_quotes_string = split_quotes_comments(line);
    printf("quotes array:\n");
    print_array(arr_quotes_string);
    if (!arr_quotes_string)
        return NULL;
    return arr_quotes_string;

/*
    tokens = ft_split_on_str(line, get_ifs_from_env(mini));
    printf("%lu\n", sizeof(tokens));
    free_args(tokens);
    arr_quotes_string = split_quotes_comments(line);
    //print_array(arr_quotes_string);
    tokens = ft_split_on_str(line, get_ifs_from_env(mini));
    print_array(tokens);
    ft_memset(&flags, 0, sizeof(t_token_flags));
    tokens_head = tokens;
    while (tokens && *tokens) {
        set_flags(&flags, tokens);
        printf("%s, in comment: %d, in_single quote: %d \n", *tokens, flags.in_comment, flags.in_single_quote);
        tokens++;
    }
    return (arr_quotes_string);*/
}




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
