/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_central.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/19 14:29:52 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	blt_central(t_execlist *execl, int i, char **exec_str, int *err)
{
	ft_printf("IM INSIDE BUILTFT\n");
	if (ft_strncmp(exec_str[0], "cd", 3) == 0)
		ft_cd(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
	else if (ft_strncmp(exec_str[0], "echo", 5) == 0)
		ft_echo(err, execl->chunk[i]->cmd_n_args);
	else if (ft_strncmp(exec_str[0], "env", 4) == 0)
		ft_env(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "exit", 5) == 0)
		ft_exit(execl->chunk[i]->cmd_n_args);
    else if (ft_strncmp(exec_str[0], "export", 7) == 0)
		ft_export(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "pwd", 4) == 0)
		ft_pwd(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "unset", 6) == 0)
		ft_unset(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
	//return (err); //caso seja preciso, mas nao me parece
	//*err_stt = err;
}

/*
- provavelmente nem preciso de ter o err aqui porque o ruben
apenas coloca a 0 maior parte das vezes e eu nem sequer preciso disso
- mudar os numeros e mensages de erro para coisas minimamente apresentaveis
ainda me reprovam c aquilo

dup2 ja vem feitos em termos de redirection
read from STDIN_FILENO
write to STDOUT_FILENO
*/

/*
int	main(int ac, char **av, char **envp)
{
	//int	builtin;
	//builtin = ft_atoi(av[1]);
	(void)ac;
	ft_printf("IM INSIDE BUILTFT\n");
	if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_cd((t_mini *)av[2], (char **)av[3], envp);
	else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_echo((t_mini *)av[2], envp);
	else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_env((t_mini *)av[2], (char **)av[3], envp);
    else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_exit((char **)av[3]);
    else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_export((t_mini *)av[2], (char **)av[3], envp);
    else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_pwd((t_mini *)av[2], (char **)av[3], envp);
    else if (ft_strncmp(av[1], "cd", 3) == 0)
		ft_unset((t_mini *)av[2], (char **)av[3], envp);
}
*/

/*
int execbi(t_mini *ms, char **cmd, char **envp)
{
    if (!ms || !cmd || !envp)
        return (-1);
    if (!ft_strncmp(*cmd, "echo", 5))
        ft_echo(ms, cmd);
    else if (!ft_strncmp(*cmd, "cd", 3))
        ft_cd(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "pwd", 4))
        ft_pwd(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "export", 7))
        ft_export(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "unset", 6))
        ft_unset(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "env", 4))
        ft_env(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "exit", 5))
        ft_exit(cmd);
    else
        return (0);
    return (1);
}
*/