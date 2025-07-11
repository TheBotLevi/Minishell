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


void free_tokens(t_token **tokens) {
    t_token *current;
    t_token *next;

    current = *tokens;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

int create_basic_tokens(char *line, t_token **tokens, t_mini *mini) {
    int i;
    t_token *token;
    t_token *prev;
    (void) mini;

    i = 0;
    prev = NULL;
    while (line[i]) {
        token = (t_token *)malloc(sizeof(t_token));
        if (!token) {
            free_tokens(tokens);
            return (1);
        }
        ft_memset(token, 0, sizeof(t_token));
        token->c = line[i];
        token->next = NULL;
        token->prev = prev;
        if (prev)
            prev->next = token;
        prev = token;
        if (i == 0)
            *tokens = prev;
        i++;
    }
    return (0);
}

int set_pipe_flags(t_token **tokens) {
    t_token *current;
    int n_pipes;

    n_pipes = 0;
    current = *tokens;
    while (current && !current->is_comment) {
        if (current->c == '|' && !current->is_quote) {
            current->is_pipe = 1;
            n_pipes++;
        }
        current = current->next;
    }
    return (n_pipes);
}


/*
< should redirect input
> should redirect output
>> should redirect output in append mode.
<< should be given a delimiter, then read the input until a line containing the
delimiter is seen. However, it doesn’t have to update the history!
 */
void set_middle_redirection_flags(t_token *current, t_token *prev, t_token *next) {

    if (current->c == '<' && prev->c != '<' && next->c != '<')
        current->is_redir_input = 1;
    else if (current->c == '>' && prev->c != '>' && next->c != '>')
        current->is_redir_output = 1;
    else if (current->c == '>' && prev->c == '>' && !prev->is_redir_output){
        prev->is_redir_output_append = 1;
        prev->is_redirection = 1;
        current->is_redir_output_append = 1;
    }
    else if (current->c == '<' && prev->c == '<' && !prev->is_redir_input){
        prev->is_redir_heredoc = 1;
        prev->is_redirection = 1;
        current->is_redir_heredoc = 1;
        while (next) {
            next->is_redirection = 1;
            next->is_redir_heredoc_delimiter = 1;
            next = next->next;
        }
    }
    else
        current->is_redirection = 0;
}

/*set redirection flags for first or last elements of the line
 * case: beginning of line: !prev && next
 * case: end of line: !next && prev
 */
void set_ends_redirection_flags(t_token *current, t_token *prev, t_token *next) {

    if (!prev && next)
        {
        if (current->c == '<' && next->c != '<')
            current->is_redir_input = 1;
        else if (current->c == '>' && next->c != '>')
            current->is_redir_output = 1;
        else
            current->is_redirection = 0;
    }
    if (!next && prev)
        {
        if (current->c == '<' && prev->c != '<')
            current->is_redir_input = 1;
        else if (current->c == '>' && prev->c != '>')
            current->is_redir_output = 1;
        else if (current->c == '>' && prev->c == '>' && !prev->is_redir_output){
            prev->is_redir_output_append = 1;
            prev->is_redirection = 1;
            current->is_redir_output_append = 1;
        }
        else if (current->c == '<' && prev->c == '<' && !prev->is_redir_input){
            prev->is_redir_heredoc = 1;
            prev->is_redirection = 1;
            current->is_redir_heredoc = 1;
        }
        else
            current->is_redirection = 0;
    }
}

void set_redirection_flags(t_token **tokens) {
    t_token *current;
    t_token *prev;
    t_token *next;

    current = *tokens;
    while (current && !current->is_comment) {
        prev = current->prev;
        next = current->next;
        if (is_in_set(current->c, "<>") && !current->is_quote) {
            current->is_redirection = 1;
            if (prev && next)
                set_middle_redirection_flags(current, prev, next);
            else
                set_ends_redirection_flags(current, prev, next);
        }
        current = next;
    }
}

void mark_braced_var(t_token *current, t_token *next) {
    t_token *start;
    //t_token *end;
    (void)current;

    start = next->prev;
    if (next && next->c == '{') {
        next = next->next;
        if (next && (ft_isalpha(next->c) || next->c == '_')) {
            next->is_var = 1;
            next = next->next;
            while (next && next->c != '}' && (ft_isalnum(next->c) || next->c == '_'))
                next = next->next;
            if (next && next->c == '}') {
                current = next;
                while (next && next != start) {
                    next->is_braced_var = 1;
                    next->is_var = 1;
                    next = next->prev;
                }
            }
        }
    }
}

void set_var_expansion_flags(t_token **tokens) {
    t_token *current;
    t_token *next;

    current = *tokens;
    while (current && !current->is_comment ) {
        next = current->next;
        if (current->c == '$' && !current->is_single_quote) {
            current->is_dollar = 1;
            if (next && next->c == '?') {
                current->is_exit_status = 1;
                next->is_exit_status = 1;
                next->is_var = 1;
            }
            else if (next && (ft_isalpha(next->c) || next->c == '_')) {
                next->is_var = 1;
                next = next->next;
                while (next && (ft_isalnum(next->c) || next->c == '_')) {
                    next->is_var = 1;
                    next = next->next;
                }
                current = next; // skip to after var name
            }
            else
                mark_braced_var(current, next);
        }
        if (current)
            current = current->next;
    }
}

void set_ifs_flags(t_mini *mini, t_token **tokens) {
    t_token *current;
    char *ifs;

    current = *tokens;
    ifs = get_ifs_from_env(mini);
    while (current && !current->is_comment) {
        if (is_in_set(current->c, ifs) && !current->is_quote)
            current->is_ifs = 1;
        current = current->next;
        }
    }


void set_token_flags(t_token **tokens, t_mini *mini) {
    t_token *current;
    int n_pipes;
    (void) mini;

    set_quote_flags(tokens);
    mark_comment(tokens);
    n_pipes = set_pipe_flags(tokens);
    printf("n_pipes: %d\n", n_pipes);
    set_redirection_flags(tokens);
    set_var_expansion_flags(tokens);
    set_ifs_flags(mini, tokens);
    current = *tokens;
    while (current) {
        printf("%c: ifs:%d, quote:%d, \':%d, \":%d, start\":%d, end\":%d, #_start:%d, #:%d,"
               " |:%d, $:%d, $?:%d, $var:%d, ${}:%d, <:%d, <<:%d, <<_del: %d, >:%d, >>:%d \n",
               current->c, current->is_ifs,current->is_quote, current->is_single_quote, current->is_double_quote, current->is_start_quote,
               current->is_end_quote,current->is_comment_start, current->is_comment, current->is_pipe, current->is_dollar,
               current->is_exit_status, current->is_var, current->is_braced_var, current->is_redir_input,
               current->is_redir_heredoc, current->is_redir_heredoc_delimiter, current->is_redir_output, current->is_redir_output_append);
        current = current->next;
    }
}

void tokenize(char *line, t_token **tokens, t_mini *mini) {
    /*int i;
    t_token *token;
    t_token *prev;
*/

    if (!line || !mini)
        return ;
    if (create_basic_tokens(line, tokens, mini) == 0)
        set_token_flags(tokens, mini);
    // todo perform variable substitution
}

t_token** split_line(char *line, t_mini *mini) {
    //char **arr_quotes_string;
    //t_tok_data *tok_data;
    t_token **tokens;
    (void) mini;
    //char **tokens_head;
    //char **tokens;
    //t_token_flags flags;

    if (!line || !mini)
        return NULL;
    tokens = (t_token **)malloc(sizeof(t_token*));
    if (!tokens)
        return NULL;
    *tokens = NULL;
    tokenize(line, tokens, mini);
    /*tok_data = split_quotes_comments(line);
    arr_quotes_string = tok_data->ar;
    printf("quotes array:\n");
    print_array(arr_quotes_string);
    if (!arr_quotes_string)
        return NULL;
    }
    free_tok_data(tok_data);*/
    return tokens;

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


// ########## UNUSED

//char** split_on_str

void unset_flags(t_token_flags *flags, char c, int quote_flag_set) {
    if (quote_flag_set)
        return ;
    if (c == '"' && flags->in_double_quote)
        flags->in_double_quote = 0;
    if (c == '\'' && flags->in_single_quote)
        flags->in_single_quote = 0;
}

/*
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
