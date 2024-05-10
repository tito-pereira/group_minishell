/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_pr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:39:10 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/10 17:51:12 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// PREP

/*
serao os redirs mesmo necessarios
sera a prep necessaria
o que eu faco aqui? so se for para atribuir algo ao redir, porque
os redirs nem sempre sao usados
e os fd so podem ser fechados diretamente no exec
ou faco mesmo tipo infd, outfd
ou faco tudo diretamente no exec
*/

void    close_non_related(t_execlist *execl, int **fd, int i)
{
    int c;
    
    c = -1;
    while (execl->chunk[++c] != NULL)
    {
        if (c != i && c != (i + 1))
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
    }
}

/*
closes:
input - (all - i - (i + 1)) + i;
output - (i + 1);
*/

void    write_heredoc(t_execlist *execl, char *str, int **fd, int i)
{
    int pid;

    pid = fork();
    if (pid == 0)
    {
		close_non_related(execl, fd, i);
		close(fd[i + 1][0]);
		close(fd[i + 1][1]);
		close(fd[i][0]);
		write(fd, str, ft_strlen(str));
		close(fd[i][1]);
    }
    wait(0);
}

void	exec_input(t_execlist *execl, int **fd, int **redir, int i)
{
    close_non_related(execl, fd, i);
	close(fd[i][1]); //fecha o pipe local (so escreve no proximo)
	if (execl->chunk[i]->heredoc == 1 && execl->chunk[i]->inpipe == 1) //1, heredoc valido
	{
        write_heredoc(execl, execl->chunk[i]->infile, fd, i);
        dup2(fd[i][0], STDIN_FILENO);
	}
    else if (execl->chunk[i]->heredoc == 0 && execl->chunk[i]->inpipe == 1) //1, normal infile
    {
        redir[i][0] = open(execl->chunk[i]->infile, O_RDONLY);
        dup2(redir[i][0], STDIN_FILENO);
		close(redir[i][0]); //depois de dup, fecha-se
    }
	else if (i > 0) //resto, ate ao ultimo
	{
        dup2(fd[i][0], STDIN_FILENO); //last pipe, fd[i - 1][0];
	}
    close(fd[i][0]);
}

void	exec_output(t_execlist *execl, int **fd, int **redir, int i)
{
	close(fd[i + 1][0]);
	if (execl->chunk[i]->outfile != NULL) //1, outfile
	{
		if (execl->chunk[i]->append == 1) //append
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
		else // truncate
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
		dup2(redir[i][1], STDOUT_FILENO);
		close(redir[i][1]); //depois de dup, fecha-se
		if ((i + 1) < execl->cmd_nmb) //outfile inside pipeline
		{
			close(fd[i + 1][1]);
			fd[i + 1][1] = redir[i][1];
			return ;
		}
	}
	else if ((i + 1) < execl->cmd_nmb && execl->chunk[i]->outfile == NULL) //2, inside pipeline, non outfile
		dup2(fd[i + 1][1], STDOUT_FILENO); //not even necessary maybe
	close(fd[i + 1][1]);
}
// if last comand, da os frees, salta tudo, e usa STDOUT 

void	prep_loop(t_execlist *execl, int **fd, int **redir, char **exec_str)
{
    int i;

    i = -1;
	while (execl->chunk[++i] != NULL)
	{
		prep_input(execl, fd, redir, i);
		prep_output(execl, fd, redir, i);
	}
}

/*
talvez fazer prep das exec_str aqui idk

as execs ja estao feitas
tirando as exec_str, ver se ha alguma ponta solta que falte
*/