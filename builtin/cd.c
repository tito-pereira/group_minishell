/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:04 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 00:08:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	go_to_dir(t_mini *ms, char *dir, char **env)
{
	char	**exp;
	char	*temp;

	temp = '\0';
	exp = ft_calloc(3, sizeof(char *));
	exp[0] = ft_strdup("export");
	if (chdir(dir) == 0)
	{
		temp = getcwd(temp, PATH_MAX);
		exp[1] = ft_strjoin("PWD=", temp);
		ft_export (ms, exp, env);
		ms->error = 0;
	}
	else
	{
		ft_putstr_fd(NPROMPT": cd: No such file or directory", 2);
		ms->error = 69;
	}
	temp = free_ptr(temp);
	exp = free_mat(exp);
}

char	*home_dir(char **env)
{
	char	*home;

	while(env && ft_strncmp(*env, "HOME=", 5))
		env++;
	home = *env + 5;
	return (home);
}

void	ft_cd(t_mini *ms, char **cmd, char **env)
{
	if (!cmd[1])
		go_to_dir(ms, home_dir(env), env);
	else if (cmd[1] && !cmd[2])
		go_to_dir(ms, cmd[1], env);
	else
	{
		ft_putstr_fd ("-" NPROMPT ": cd:  invalid usage\n", 2);
		ms->error = 69;
	}
}
