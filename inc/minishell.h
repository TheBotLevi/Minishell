/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:21 by ljeribha          #+#    #+#             */
/*   Updated: 2025/07/09 12:02:43 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include <fcntl.h>

# define BUFFER_SIZE 4096

typedef struct s_mini t_mini;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/*typedef struct s_pipeline
{
	char			*commands; //changed from t_mini to char for clean code purposes
	int				cmd_count;
	int				**pipes;
	pid_t			*pids;
}					t_pipeline;*/

typedef struct s_command {
	char *cmd;
	char *fin_args;
	char **args;
	/* char *redir_in;
	 char *redir_out;
	 char *redir_append;
	 char *heredoc;
	 char *delimiter;
	 char *cmd_str;
	 int is_builtin;*/
} t_command;

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

typedef struct s_token_data {
} t_token_data;

typedef struct s_mini
{
	char			**args;	//is going to change for parsing (tokenization)
	char			*old_path;
	int				input_fd;
	int				output_fd;
	int				fd;
	char			*filename;
	int				exit_status;
	t_mini			*commands;
	int				cmd_count;
	int				**pipes;
	int			saved_stdin;
	int			saved_stdout;
	int			redir_flag;
	pid_t			*pids;
	t_env		*env_struct;
//	t_pipeline	*pipes;
//	t_list		*list;
	struct s_mini	*next;
}					t_mini;

typedef struct s_token {
	char c;
	int is_quote;
	int is_single_quote;
	int is_double_quote;
	int is_start_quote;
	int is_end_quote;
	int is_cmd_sep;
	int is_pipe;
	int is_ifs;
	int is_dollar;
	int is_var;
	int is_braced_var;
	int is_exit_status;
	//int is_eof; // signals ctrl+D
	int is_comment_start;
	int is_comment;
	int is_redir_heredoc;
	int is_redir_heredoc_delim_start;
	int is_redir_heredoc_delim_end;
	int is_redir_heredoc_delimiter;
	//int is_heredoc_end;
	int is_redirection;
	int is_redirection_end;
	int is_redir_input;
	int is_redir_output;
	int is_redir_output_append;
	struct s_token *prev;
	struct s_token *next;
} t_token;

typedef struct s_quote_state {
	int in_single_quote;
	int in_double_quote;
	int within_quote;
	int is_start_quote;
	int is_end_quote;
} t_quote_state;


typedef struct s_tok_ar {
	char *elem;
	struct s_tok_ar *next;
	int is_quote;
	int is_single_quote;
	int is_double_quote;
}t_tok_ar;

typedef struct s_tok_data {
	char *line;
	size_t n_elems;
	int *in_quote_arr;
	int n_splits;
	int*	size_arr;
	char **ar;
	//t_tok_ar *ar;
} t_tok_data;



//global variable
extern volatile sig_atomic_t	g_exit;

// main
void				ft_mini_loop(t_mini *mini);

// utils
int					ft_strcmp(char *s1, char *s2);
void				clear_readline_history(void);
char				*get_env_value(t_env *env, char *key);
char				**parse_input(char *line);
void				sort_env_vars(t_env **sorted_env, int count);
int					is_valid_export(char *str);
void	update_exit_status(t_mini *mini);
int	add_new_env_var(t_env **env, char *key, char *value);
void print_array(char** ar);

// built_in's
int					mini_echo(char **args);
int					mini_exit(t_mini *mini);
int					mini_pwd(void);
int					mini_unset(t_mini *mini);
int					mini_env(t_env **env);
int					mini_cd(t_mini *mini);
int					mini_export(t_mini *mini);

// handling built_in's
int					is_builtin(char *cmd);
int					handle_builtin(t_mini *mini);

// remove env
int					remove_env_head(t_env **env, char *key);
int					remove_env_var(t_env **env, char *key);

// add or update env to list
int					update_env(t_env **env, char *var_str);

// env
char				**env_list_to_array(t_env *env_list);
char	**get_paths_from_list(t_env *env_list);
t_env	*create_env_list(char **env);

// free
void				free_n_array(char **ar, size_t i);
void				free_args(char **args);
void				free_env_list(t_env *env);
void	free_command_list(t_mini *cmds);
void	free_everything(t_mini *mini);

// execute
int					execute_external_cmd(t_mini *mini);
int	process_command(char *line, t_mini *mini);
char	*find_exec(char *cmd, char **paths);

//signals
void	setup_signals(void);
void	setup_child_signals(void);
void	handle_sigint(int sig);
void	handle_heredoc_sigint(int sig);
void	handle_heredoc_signals(void);
void	restore_main_signals(void);

//init
t_env	*init_environment(char **env);
t_mini	*mini_init(char **envp);

//update
int	update_env_value(t_env **env, char *key, char *value);

// pipes
int		has_pipes(char *line);
int		parse_pipeline(char *line, t_mini **pipeline);
int		execute_pipeline(t_mini *pipeline);
void	free_pipeline(t_mini *pipeline);

//redirections
int	execute_redirections(t_mini *mini);
int	handle_heredoc_redirection(t_mini *mini, char *delimiter);

//parsing
char* get_ifs_from_env(t_mini *mini);
char	**ft_split_on_str(char const *s, char const *c);
t_token** split_line(char *line, t_mini *mini);
int *get_quote_state_array(char const *str);
int is_within_quote(char c, t_quote_state *state);
size_t get_int_array_size(const int *arr);
t_tok_data *split_quotes_comments(char const *line);
void free_tok_data(t_tok_data *tok_data);
void cancel_non_quote_comment(char const *str, int *in_quote_arr);
int set_quote_flags(t_token **tokens);
int is_within_quote_token(const char c, t_quote_state *state);
void cancel_unfinished_quote_token(t_token *token);
void mark_comment(t_token **tokens);
void free_tokens(t_token **tokens);

#endif
