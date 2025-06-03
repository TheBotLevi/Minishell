/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:25:21 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/02 13:40:54 by ljeribha         ###   ########.fr       */
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
void	cmd_pwd(void);

//built_in's
int	mini_echo(char **args);

//handling built_in's
int	is_builtin(char *cmd);
int	handle_builtin(char **cmd, t_env *env);

#endif