/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:21 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/08 20:52:26 by ljeribha         ###   ########.fr       */
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

# define BUFFER_SIZE 4096

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// main
void				ft_mini_loop(t_env *env_list);

// utils
int					ft_strcmp(char *s1, char *s2);
void				surpress_rl_leaks(void);
char				*get_env_value(t_env *env, char *key);
char				**parse_input(char *line);
void				sort_env_vars(t_env **sorted_env, int count);
int					is_valid_export(char *str);
void	update_exit_status(t_env **env, int status);
// void	cmd_pwd(void);

// built_in's
int					mini_echo(char **args);
int					mini_exit(char **args);
int					mini_pwd(void);
int					mini_unset(char **cmds, t_env **env);
int					mini_env(t_env **env);
int					mini_cd(char **args, t_env **env);
int					mini_export(char **args, t_env **env);

// handling built_in's
int					is_builtin(char *cmd);
int					handle_builtin(char **cmd, t_env **env);

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
int					execute_external_cmd(char **args, t_env *env);
int	process_command(char *line, t_env **env);
char	*find_exec(char *cmd, char **paths);

//signals
void	setup_signals(void);

//init
t_env	*init_environment(char **env);

//update
int	update_env_value(t_env **env, char *key, char *value);

#endif