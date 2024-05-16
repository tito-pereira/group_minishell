/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/16 18:06:35 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**/
int	main(int ac, char **av, char **envp)
{
	int		err;
	char	*cmds;

	(void)ac;
	err = 0;
	ft_printf("IM INSIDE BUILTFT\n");
	//av[1] vai ser o pipe
	if (ft_strncmp(av[2], "cd", 3) == 0)
		ft_cd(&err, envp);
	else if (ft_strncmp(av[2], "echo", 5) == 0)
		ft_echo(&err, envp);
	else if (ft_strncmp(av[2], "env", 4) == 0)
		ft_env(&err, envp);
    else if (ft_strncmp(av[2], "exit", 5) == 0)
		ft_exit(&err,);
    else if (ft_strncmp(av[2], "export", 7) == 0)
		ft_export(&err, envp);
    else if (ft_strncmp(av[2], "pwd", 4) == 0)
		ft_pwd(&err, envp);
    else if (ft_strncmp(av[2], "unset", 6) == 0)
		ft_unset(&err, envp);
	// write envp to pipe
	// free envp (porque, ao mexer, vai ser duplicada)
	return (err);
}

/*
se calhar apenas ignorar os error status porque nao
ha maneira de comunicar com o main process

e como comunico os execves?
o sistema em si faz errno
- sera que existe alguma getenv de errno?
t_mini pelo error status?

- como comunicar mudancas ao envp ao main process (pipes?)
eu tenho envp alocado
se eu passar essa morada
o fork apenas cria uma copia do endereco mas o local fisico é o mesmo
porque foi dado com malloc e 

dito isto, o envp é capaz de ser copiado e eu quero manter as mudancas
which means, another pipe e o retorno
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