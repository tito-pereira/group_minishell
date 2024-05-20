/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_outf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:25:54 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/20 02:44:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ex_redir_file(t_execlist *execl, int i, char ***exec_str, char *buff, int *nfd)
{
	int	tmp;

	tmp = 0;
	if (buff)
		temp_pipe(nfd, buff);
	if (execl->chunk[i]->append == 1) //append redir[i][1]
		tmp = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
	else // truncate
		tmp = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
	dup2(tmp, STDOUT_FILENO);
	close(tmp); //depois de dup, fecha-se
	execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
}

void	ex_redir_pipe(int **fd, int i, char *buff, int *nfd)
{
	if (buff)
		temp_pipe(nfd, buff);
	dup2(fd[i + 1][1], STDOUT_FILENO);
	close(fd[i + 1][1]);
	if (buff)
		free(buff);
	free(nfd);
}

void	ex_end(char *buff, int *nfd)
{
	if (buff)
		free(buff);
	free(nfd);
	exit(0);
}

// cat<example.txt>tmp1|cat -e>tmp2|cat -e>tmp3

void	ex_outfile(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	char	*buff;
	int		*nfd;
	int 	pid;
	
	//printf("inside outfile in [%d]\n", i);
	buff = empty_pipe(fd[i][0], execl, i);
	//printf("buff:%s;\n", buff);
	nfd = ft_calloc(2, sizeof(int));
	pid = fork();
	if (pid == 0)
	{
		if ((i + 1) < execl->valid_cmds)
			close(fd[i + 1][1]);
		ex_redir_file(execl, i, exec_str, buff, nfd);
	}
	wait(NULL);
	if ((i + 1) < execl->valid_cmds) //outfile inside pipeline
	{
		ex_redir_pipe(fd, i, buff, nfd);
		execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
	}
	else
		ex_end(buff, nfd);
}