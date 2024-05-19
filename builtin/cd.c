/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:04 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/19 14:49:29 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	go_to_dir(int *err, char *dir, char ***env)
{
	char	**exp;
	char	*temp;

	temp = NULL;
	exp = ft_calloc(3, sizeof(char *));
	exp[0] = ft_strdup("export");
	if (chdir(dir) == 0)
	{
		temp = getcwd(temp, PATH_MAX);
		exp[1] = ft_strjoin("PWD=", temp);
		ft_export (err, exp, env);
		*err = 0;
	}
	else
	{
		ft_putstr_fd(NPROMPT": cd: No such file or directory", 2);
		*err = 69;
	}
	temp = free_str(temp);
	exp = free_db_str(exp);
}

char	*home_dir(char ***env)
{
	char	*home;

	while(env && ft_strncmp(*env, "HOME=", 5))
		env++;
	home = *env + 5;
	return (home);
}

void	ft_cd(int *err, char **cmd, char ***env)
{
	if (!cmd[1])
		go_to_dir(err, home_dir(env), env);
	else if (cmd[1] && !cmd[2])
		go_to_dir(err, cmd[1], env);
	else
	{
		ft_putstr_fd ("-" NPROMPT ": cd:  invalid usage\n", 2);
		*err = 69;
	}
}
