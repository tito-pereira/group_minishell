/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/04/30 13:39:28 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


int	main(int ac, char **av, char **envp)
{
	int	builtin;

	if (av[1] == 1)
		ft_cd((t_mini *)av[2], (char **)av[3], envp);
	else if (av[1] == 2)
		ft_echo((t_mini *)av[2], envp);
	else if (av[1] == 3)
		ft_env((t_mini *)av[2], (char **)av[3], envp);
    else if (av[1] == 4)
		ft_exit((t_mini *)av[2], envp);
    else if (av[1] == 5)
		ft_export((t_mini *)av[2], (char **)av[3], envp);
    else if (av[1] == 6)
		ft_pwd((t_mini *)av[2], (char **)av[3], envp);
    else if (av[1] == 7)
		ft_unset((t_mini *)av[2], (char **)av[3], envp);
}

/*
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