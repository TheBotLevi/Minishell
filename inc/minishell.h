/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:21 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/30 11:22:13 by ljeribha         ###   ########.fr       */
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

typedef struct s_pipeline
{
	t_mini			*commands;
	int				cmd_count;
	int				**pipes;
	pid_t			*pids;
}					t_pipeline;

typedef struct s_mini
{
	char			**args;	//is going to change for parsing (tokenization)
	char			*old_path;
	int				input_fd;
	int				output_fd;
	int				fd;
	char			*filename;
	int				exit_status;
	t_env		*env_struct;
	t_pipeline	*pipes;
	t_list		*list;
	struct s_mini	*next;
}					t_mini;

//global variable
extern volatile sig_atomic_t	g_exit;

// main
void				ft_mini_loop(t_mini *mini);

// utils
int					ft_strcmp(char *s1, char *s2);
void				surpress_rl_leaks(void);
char				*get_env_value(t_env *env, char *key);
char				**parse_input(char *line);
void				sort_env_vars(t_env **sorted_env, int count);
int					is_valid_export(char *str);
void	update_exit_status(t_mini *mini);
int	add_new_env_var(t_env **env, char *key, char *value);

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
void				free_args(char **args);
void				free_env_list(t_env *env);

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
int		parse_pipeline(char *line, t_pipeline **pipeline);
int		execute_pipeline(t_pipeline *pipeline);
void	free_pipeline(t_pipeline *pipeline);

//redirections
int	execute_redirections(t_mini *mini);
int	handle_heredoc_redirection(t_mini *mini, char *delimiter);

#endif