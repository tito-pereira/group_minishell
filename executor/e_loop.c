/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/12 13:08:24 by tibarbos         ###   ########.fr       */
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

	//ft_printf("Launching command[%d](cmds %d)\n", i, execl->valid_cmds);
	if ((i + 1) < execl->valid_cmds)
	{
		i++;
		//ft_printf("creating next command[%d](cmds %d)\n", i, execl->valid_cmds);
		pid = fork();
		if (pid == 0)
		{
			exec_launch(execl, fd, redir, i, exec_str);
			exit(0);
		}
		i--;
	}
	pid = fork();
	if (pid == 0)
	{
		exec_input(execl, fd, redir, i);
		exec_output(execl, fd, redir, i);
		execve(exec_str[i][0], exec_str[i], execl->my_envp);
		ft_printf("exec action failed [%d]\n", i);
	}
	else
	{
		ft_printf("lets wait for first one[%d]\n", i);
		pid = wait(0);
		//ft_printf("----------------- DONE EXECUTE -----------------\n");
		//printf("Waiting for own process[%d] to finish with status %d\n", i, pid);
		if ((i + 1) < execl->valid_cmds)
		{
			ft_printf("lets wait for scnd one[%d]\n", i);
			wait(0);
			//ft_printf("Waiting for child process[%d] to finish\n", (i + 1));
		}
		ft_printf("launch done and waited [%d]\n", i);
	}
	ft_printf("exec launch out [%d]\n", i);
}

/*
ls -1 |cat|cat

lets wait for first one[0]
lets wait for first one[1]
lets wait for first one[2]
lets wait for scnd one[0]
launch done and waited [2]
exec launch out [2]
lets wait for scnd one[1]

o wait for second one fica constantemente preso
sera que posso usar varios waits?
ou sera outra merda qq
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