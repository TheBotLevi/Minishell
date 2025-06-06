/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:08:25 by ljeribha          #+#    #+#             */
/*   Updated: 2025/06/05 15:23:38 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_pwd_vars(t_env **env, char *old_path)
{
	char	current_dir[BUFFER_SIZE];
	char	*oldpwd_str;
	char	*pwd_str;

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
		return ;
	oldpwd_str = ft_strjoin("OLDPWD=", old_path);
	if (oldpwd_str)
	{
		update_env(env, oldpwd_str);
		free(oldpwd_str);
	}
	pwd_str = ft_strjoin("PWD=", current_dir);
	if (pwd_str)
	{
		update_env(env, pwd_str);
		free(pwd_str);
	}
}

static char	*get_cd_path(char **args, t_env **env)
{
	char	*path;

	if (!args[1] && ft_strcmp(args[1], "~") == 0)
		path = get_env_value(*env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(*env, "OLDPWD");
		if (path)
			ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
		path = args[1];
	return (path);
}

static int	handle_cd_errors(char **args, char *path, char *old_path)
{
	ft_putstr_fd("minishell: cd ", STDERR_FILENO);
	if (!path && !args[1])
		ft_putendl_fd("HOME not set", STDERR_FILENO);
	else
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
	free(old_path);
	return (1);
}

int	mini_cd(char **args, t_env **env)
{
	char	*path;
	char	current_dir[BUFFER_SIZE];
	char	*old_path;

	if (getcwd(current_dir, BUFFER_SIZE) == NULL)
		return (1);
	old_path = ft_strdup(current_dir);
	if (!old_path)
		return (1);
	path = get_cd_path(args, env);
	if ((!path && !args[1]) || (path && chdir(path) == -1))
		return (handle_cd_errors(args, path, old_path));
	update_pwd_vars(env, old_path);
	free(old_path);
	return (0);
}
