/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:08:25 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/27 11:22:50 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_pwd_vars(t_mini *mini)
{
	char	current_dir[BUFFER_SIZE];
	char	*oldpwd_str;
	char	*pwd_str;

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
		return ;
	oldpwd_str = ft_strjoin("OLDPWD=", mini->old_path);
	if (oldpwd_str)
	{
		update_env(mini->env_struct, oldpwd_str);
		free(oldpwd_str);
	}
	pwd_str = ft_strjoin("PWD=", current_dir);
	if (pwd_str)
	{
		update_env(mini->env_struct, pwd_str);
		free(pwd_str);
	}
}

static char	*get_cd_path(t_mini *mini)
{
	char	*path;

	if (!mini->args[1] || ft_strcmp(mini->args[1], "~") == 0)
		path = get_env_value(*(mini->env_struct), "HOME");
	else if (ft_strcmp(mini->args[1], "-") == 0)
	{
		path = get_env_value(*(mini->env_struct), "OLDPWD");
		if (path)
			ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
		path = mini->args[1];
	return (path);
}

static int	handle_cd_errors(t_mini *mini, char *path)
{
	ft_putstr_fd("minishell: cd ", STDERR_FILENO);
	if (!path && !mini->args[1])
		ft_putendl_fd("HOME not set", STDERR_FILENO);
	else
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
	free(mini->old_path);
	return (1);
}

int	mini_cd(t_mini *mini)
{
	char	*path;
	char	current_dir[BUFFER_SIZE];

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
		return (1);
	mini->old_path = ft_strdup(current_dir);
	if (!mini->old_path)
		return (1);
	path = get_cd_path(mini);
	if ((!path && !mini->args[1]) || (path && chdir(path) == -1))
		return (handle_cd_errors(mini, path));
	update_pwd_vars(mini);
	free(mini->old_path);
	return (0);
}
