/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_ex.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/10 17:14:53 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
nao esquecer do heredoc
pode haver um pipe antes do primeiro comando com os heredocs
*/

void	exec_input(t_execlist *execl, int **fd, int **redir, int i)
{
	close(fd[i][0]); //assume posicao de escrita do pipe
	if (execl->chunk[i]->infile != NULL && execl->chunk[i]->inpipe == 1) //1, infile redir valido
	{
		redir[i][0] = open(execl->chunk[i]->infile, O_RDONLY);
		dup2(redir[i][0], STDIN_FILENO);
		close(redir[i][0]); //depois de dup, fecha-se
	}
	else if (i > 0) //resto, inpipe redir invalido, comando nº>1, recebe smp pipe
	{
		dup2(execl->chunk[i]->inpfd, STDIN_FILENO);
		close (execl->chunk[i]->inpfd); // se tiver que ser usado, é usado antes daqui
	}
	//acho que se fecha o original depois da dup certo?
	// else, normal input sem dup
}

void	exec_output(t_execlist *execl, int **fd, int **redir, int i)
{
	if (execl->chunk[i]->outfile != NULL) //1, outfile redir
	{
		dup2(redir[i][1], STDOUT_FILENO);
		close(redir[i][1]); //depois de dup, fecha-se
	}
	else if ((i + 1) < execl->cmd_nmb && execl->chunk[i]->outfile != NULL) //2, next chunk redir
	{
		dup2(fd[i][1], STDOUT_FILENO);
	}
	close(fd[i][1]);
}

/*
estes nao vao ser tao grandes quanto os prep
aqui faco as dup2 + exec, nada demais

a parte importante de separar é mesmo porque cada dup2 e execve tem de ser
especificos ao processo em que sao chamados
*/

void	exec_action(t_execlist *execl, int **fd, int **redir, int i, char **exec_str)
{
	exec_input(execl, fd, redir, i);
	exec_output(execl, fd, redir, i);
	execve(exec_str[0], exec_str, *(execl->my_envp));
}

void	exec_launch(t_execlist *execl, int **fd, int **redir, int i, char ***exec_str)
{
	int	pid;

	if ((i + 1) < execl->valid_cmds)
	{
		i++;
		pid = fork();
		if (pid == 0)
			exec_action(execl, fd, redir, i, exec_str[i]);
		i--;
	}
	pid = fork();
	if (pid == 0)
		exec_action(execl, fd, redir, i, exec_str[i]);
	wait(0);
	if ((i + 1) < execl->valid_cmds)
		wait(0);
}

void	exec_loop(t_execlist *execl, int **fd, int **redir, char ***exec_str)
{
	int	i;
	int	pid;

	i = 0;
	pid = fork();
	if (pid == 0)
		exec_launch(execl, fd, redir, i, exec_str);
	wait(0);
}