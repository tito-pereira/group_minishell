/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_pr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:39:10 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/10 15:13:15 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// PREP

void	prep_input(t_execlist *execl, int **fd, int **redir, int i)
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

void	prep_output(t_execlist *execl, int **fd, int **redir, int i)
{
	if (execl->chunk[i]->outfile != NULL) //1, outfile redir
	{
		if (execl->chunk[i]->append == 1)
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
		else
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if ((i + 1) < execl->cmd_nmb) //outfile inside pipeline
			execl->chunk[i + 1]->inpfd = redir[i][1];
		dup2(redir[i][1], STDOUT_FILENO);
		close(redir[i][1]); //depois de dup, fecha-se
	}
	else if ((i + 1) < execl->cmd_nmb && execl->chunk[i]->outfile != NULL) //2, next chunk redir
	{
		execl->chunk[i + 1]->inpfd = fd[1];
		dup2(fd[i][1], STDOUT_FILENO);
	}
	close(fd[i][1]);
}

void	prep_loop(t_execlist *, int **fd, int **redir, char **exec_str)
{
	while (execl->chunk[++i] != NULL) //&& *error_stt != 126)
	{
		input_options(execl, fd, redir, i);
		output_options(execl, fd, redir, i);
	}
}