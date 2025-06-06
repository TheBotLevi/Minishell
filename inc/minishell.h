/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:21 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/06 16:10:45 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

#define BUFFER_SIZE 4096

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;


//main
void	ft_mini_loop();

//utils
int	ft_strcmp(char *s1, char *s2);
void	surpress_rl_leaks(void);
char	*get_env_value(t_env *env, char *key);
char	**parse_input(char *line);
void	sort_env_vars(t_env **sorted_env, int count);
//void	cmd_pwd(void);

//built_in's
int	mini_echo(char **args);
int	mini_exit(int ac, char **args);
int	mini_pwd(void);
int	mini_unset(char **cmds, t_env **env);
void	mini_env(t_env **env);

//handling built_in's
int	is_builtin(char *cmd);
int	handle_builtin(char **cmd, t_env *env);

//remove env
int	remove_env_head(t_env **env, char *key);
int	remove_env_var(t_env **env, char *key);

//add or update env to list
int	update_env(t_env **env, char *var_str);

#endif