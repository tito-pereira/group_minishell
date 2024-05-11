/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/11 17:41:34 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

////////////////////////////////////////////

void	exec_action(t_execlist *execl, int **fd, int **redir, int i, char **exec_str)
{
	//ft_printf("exec action on cmd[%d]\n", i);
	exec_input(execl, fd, redir, i);
	exec_output(execl, fd, redir, i);
	//ft_printf("----------------- LETS EXECUTE -----------------\n");
	execve(exec_str[0], exec_str, execl->my_envp);
	//ft_printf("EXECVE FAILED\n");
}

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
		//ft_printf("this is so fuckind dumb[%i]\n", i);
		exec_action(execl, fd, redir, i, exec_str[i]);
	}
	else
	{
		//ft_printf("lets wait for first one[%d]\n", i);
		pid = wait(0);
		//ft_printf("----------------- DONE EXECUTE -----------------\n");
		//printf("Waiting for own process[%d] to finish with status %d\n", i, pid);
		if ((i + 1) < execl->valid_cmds)
		{
			//ft_printf("lets wait for scnd one[%d]\n", i);
			wait(0);
			//ft_printf("Waiting for child process[%d] to finish\n", (i + 1));
		}
	}
}

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