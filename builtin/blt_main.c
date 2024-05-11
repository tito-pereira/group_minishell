/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/11 11:20:55 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **envp)
{
	int	builtin;

	(void)ac;
	builtin = ft_atoi(av[1]);
	if (builtin == 1)
		ft_cd((t_mini *)av[2], (char **)av[3], envp);
	else if (builtin == 2)
		ft_echo((t_mini *)av[2], envp);
	else if (builtin == 3)
		ft_env((t_mini *)av[2], (char **)av[3], envp);
    else if (builtin == 4)
		ft_exit((char **)av[3]);
    else if (builtin == 5)
		ft_export((t_mini *)av[2], (char **)av[3], envp);
    else if (builtin == 6)
		ft_pwd((t_mini *)av[2], (char **)av[3], envp);
    else if (builtin == 7)
		ft_unset((t_mini *)av[2], (char **)av[3], envp);
}

/*
.mudar as t_mini para um int *error

av[1] = int option;
av[2] = t_mini *ms;
av[3] = char **cmd;
av[4] = char **envp;

1	ft_cd(t_mini *ms, char **cmd, char **env);
2	ft_echo(t_mini *ms, char **cmd);
3	ft_env(t_mini *ms, char **cmd, char **envp);
4	ft_exit(t_mini *ms, char **cmd);
5	ft_export(t_mini *ms, char **cmd, char **envp);
6   ft_pwd(t_mini *ms, char **cmd, char **envp);
7	ft_unset(t_mini *ms, char **cmd, char **envp);

traduzir t_mini *;
deixar um united char **args para usar aqui;
*/