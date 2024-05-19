/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/18 18:18:21 by marvin           ###   ########.fr       */
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

void	exec_action(t_execlist *execl, int **fd, int **redir, int i, char ***exec_str)
{
	exec_input(execl, fd, redir, i);
	exec_output(execl, fd, i, exec_str);
	//if (execl->chunk[i]->blt == 0)
	execve(exec_str[i][0], exec_str[i], execl->my_envp);
	//else if (execl->chunk[i]->blt == 1)
	//	blt_central(execl, i, exec_str[i]); //int *err_stt
	exit(0);
}

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
			//exit(0);
		}
		i--;
	}
	pid = fork();
	if (pid == 0)
		exec_action(execl, fd, redir, i, exec_str);
	else
	{
		close_pipes(execl, fd, i, 1, 1);
		/*close_non_related(execl, fd, i);
		close(fd[i][0]);
		close(fd[i][1]);
		if ((i + 1) < execl->valid_cmds)
		{
			close(fd[i + 1][0]);
			close(fd[i + 1][1]);
		}*/
		//ft_printf("lets wait for first one[%d]\n", i);
		pid = wait(NULL);
		if ((i + 1) < execl->valid_cmds)
		{
			//ft_printf("lets wait for scnd one[%d]\n", i);
			wait(NULL);
		}
		//ft_printf("launch done and waited [%d]\n", i);
	}
	exit(0);
	//ft_printf("exec launch out [%d]\n", i);
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
	close_pipes(execl, fd, i, 1, 1);
	/*close_non_related(execl, fd, i);
	close(fd[i][0]);
	close(fd[i][1]);
	if ((i + 1) < execl->valid_cmds)
	{
		close(fd[i + 1][0]);
		close(fd[i + 1][1]);
	}*/
	wait(NULL);
	ft_printf("\n\n\nexec_loop finished.\n");
}