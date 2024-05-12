/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/12 17:26:25 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

////////////////////////////////////////////
void	print_db(char **str)
{
	int i = -1;
	while (str[++i] != NULL)
		ft_printf("%s\n", str[i]);
}

/*void	exec_action(t_execlist *execl, int **fd, int **redir, int i, char **exec_str)
{
	//ft_printf("exec action on cmd[%d]\n", i);
	exec_input(execl, fd, redir, i);
	exec_output(execl, fd, redir, i);
	//ft_printf("----------------- LETS EXECUTE -----------------\n");
	//ft_printf("my_envp:\n");
	//print_db(execl->my_envp);
	//ft_printf("\nnormal envp:\n");
	//print_db(__environ);
	//ft_printf("\nexec str:\n");
	//print_db(exec_str);
	execve(exec_str[0], exec_str, execl->my_envp);
	ft_printf("EXECVE FAILED\n");
}*/

void	exec_launch(t_execlist *execl, int **fd, int **redir, int i, char ***exec_str)
{
	int	pid;

	if ((i + 1) < execl->valid_cmds)
	{
		i++;
		//ft_printf("launching other [%d]\n", (i - 1));
		pid = fork();
		if (pid == 0)
		{
			exec_launch(execl, fd, redir, i, exec_str);
			//ft_printf("out of launch [%d]\n", i);
			exit(0);
		}
		i--;
	}
	pid = fork();
	if (pid == 0)
	{
		exec_input(execl, fd, redir, i);
		exec_output(execl, fd, redir, i);
		//ft_printf("exec action [%d]\n", i);
		execve(exec_str[i][0], exec_str[i], execl->my_envp);
		//ft_printf("action failed [%d]\n", i);
	}
	else
	{
		//ft_printf("lets wait for first one[%d]\n", i);
		pid = wait(0);
		if ((i + 1) < execl->valid_cmds)
		{
			//ft_printf("lets wait for scnd one[%d]\n", i);
			wait(0);
		}
		//ft_printf("launch done and waited [%d]\n", i);
	}
	exit(0);
	//ft_printf("exec launch out [%d]\n", i);
}

/*
ls -1 |cat|cat

ls -1         cat       cat
launch V    launch V   launch X 
action V    action V   action V
command[0] command[1] command[2]
wait1 (V)   wait1(V)   wait(V)
wait2 (X)   wait2(X)   wait(X)
out X        out X      out V
o_l X        o_l X      o_l V

testar ls|ls|ls
-> nao ha dups
-> nao houve mais erros
-> um dos outputs nao aparece (relativo, c mais comandos aparece)
(acho que o ultimo é que repete mesmo output) (nop)
(dois outputs normais e um redirected ffs)

vou tentar desligar os dups todos
*/

void	exec_loop(t_execlist *execl, int **fd, int **redir, char ***exec_str)
{
	int	i;
	int	pid;

	i = 0;
	ft_printf("Inside exec_loop.\n\n\n");
	pid = fork();
	if (pid == 0)
		exec_launch(execl, fd, redir, i, exec_str);
	wait(0);
	ft_printf("\n\n\nexec_loop finished.\n");
}