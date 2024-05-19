/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:04 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/19 16:54:47 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_dir(char *dir, char ***envp, int *err)
{
	char	*temp;
	char	**var;

	temp = NULL;
	var = ft_calloc (3, sizeof (char *));
	var[0] = ft_strdup ("export");
	temp = getcwd (temp, BUFFER_SIZE);
	if (chdir (dir))
	{
		ft_printf("minishell >> : cd : not a directory");
		*err = 69;
	}
	else
	{
		var[1] = ft_strjoin ("OLDPWD=", temp);
		ft_export (err, var, envp);
		temp = free_str(temp);
		var[1] = free_str(var[1]);
		temp = getcwd (temp, BUFFER_SIZE);
		var[1] = ft_strjoin ("PWD=", temp);
		ft_export (err, var, envp);
	}
	temp = free_str(temp);
	var = free_db_str(var);
}

static char	*get_home(char **envp)
{
	char	*home;

	while (envp && ft_strncmp (*envp, "HOME=", 5))
		envp++;
	home = *envp + 5;
	return (home);
}

void	ft_cd(int *err, char **cmd, char ***envp)
{
	if (cmd[1] && !cmd[2])
		set_dir (cmd[1], envp, err);
	else if (!cmd[1])
		set_dir (get_home (*envp), envp, err);
	else
	{
		ft_printf("minishell >> : cd: invalid usage\n", 2);
		*err = 69;
	}
	*err = 0;
}