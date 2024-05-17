/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_action.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:39:10 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/17 15:46:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    write_heredoc(t_execlist *execl, char *str, int **fd, int i)
{
    int pid;

	//ft_printf("writing heredoc.\n");
	pid = fork();
    if (pid == 0)
    {
		if ((i + 1) < execl->valid_cmds)
		{
			close(fd[i + 1][0]);
			close(fd[i + 1][1]);
		}
		close(fd[i][0]);
		//ft_printf("will write '%s' into pipe, with %d len\n", str, ft_strlen(str));
		//ft_printf("will write into fd[%d][1]=%d;\n", i, fd[i][1]);
		write(fd[i][1], str, ft_strlen(str));
		//ft_printf("%d bytes were written\n", ret);
		//close_pipes(execl, fd, i, 1, 0);
		close(fd[i][1]);
		exit(0);
	}
    wait(0);
}

void	exec_input(t_execlist *execl, int **fd, int **redir, int i)
{
	//ft_printf("preparing input for exec[%d]\n", i);
    close_pipes(execl, fd, i, 0, 1);
	//if (!(execl->chunk[i]->heredoc == 1 && execl->chunk[i]->inpipe == 1))
	//close(fd[i][1]); //fecha o pipe local (so escreve no proximo)
	//ft_printf("In input, closed(fd[%d][1] = %d)\n", i, fd[i][1]);
	if (execl->chunk[i]->heredoc == 1 && execl->chunk[i]->inpipe == 1) //1, heredoc valido
	{
		//ft_printf("heredoc input [%d]\n", i);
        write_heredoc(execl, execl->chunk[i]->infile, fd, i);
		//ft_printf("will read from fd[%d][0]=%d;\n", i, fd[i][0]);
		dup2(fd[i][0], STDIN_FILENO);
	}
	close(fd[i][1]); //fecha o pipe local (so escreve no proximo)
    if (execl->chunk[i]->heredoc == 0 && execl->chunk[i]->inpipe == 1
		&& execl->chunk[i]->infile != NULL) //1, normal infile
    {
		//ft_printf("infile input [%d]\n", i);
        redir[i][0] = open(execl->chunk[i]->infile, O_RDONLY);
        dup2(redir[i][0], STDIN_FILENO);
		close(redir[i][0]); //depois de dup, fecha-se
    }
	else if (i > 0) //resto, ate ao ultimo
	{
		//ft_printf("pipeline input [%d]\n", i);
        dup2(fd[i][0], STDIN_FILENO); //last pipe, fd[i - 1][0];
		//ft_printf("dup2(fd[%d][0] = %d, STDIN_FILENO = %d);\n", i, fd[i][0], STDIN_FILENO);
	}
	if (execl->chunk[i]->outfile == NULL)
    	close(fd[i][0]);
	//porque so fecho quando nao ha outfile?
}

/*
se houver outfile, eu ainda uso o input para ler para o buffer
dando dup2 ou nao consoante o main pipeline (ficando aberto 1 para o dup
ou 0 abertos pq n ha input pipe), outfile é sempre um caso especial
que precisa dele aberto anyway para copiar para a redirection
2 outputs vindos do mesmo input
*/

char	*empty_pipe(int fd)
{
	char	*shovel;
	char	*chest;
	char	*old;

	shovel = get_next_line(fd);
	chest = NULL;
	while (shovel != NULL)
	{
		if (!chest)
			chest = ft_strdup(shovel);
		else
		{
			old = chest;
			chest = ft_strjoin(chest, shovel);
			free(shovel);
			shovel = NULL;
			free(old);
		}
		shovel = get_next_line(fd);
	}
	//ft_printf("final_chest:%s;\n", chest);
	return (chest);
}

void	temp_pipe(int *nfd, int pid, char *buff)
{
	pipe(nfd);
	pid = fork();
	if (pid == 0)
	{
		close(nfd[0]);
		//ft_printf("buff:%s;", buff);
		write(nfd[1], buff, ft_strlen(buff));
		close(nfd[1]);
		exit(0);
	}
	close(nfd[1]);
	wait(NULL);
	dup2(nfd[0], STDIN_FILENO);
	close(nfd[0]);
}

void	exec_output(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	int		pid;
	int		tmp;
	char	*buff;
	int		*nfd;

	pid = -2; //ls > tmp1 | cat > tmp2 | cat > tmp3 | cat > tmp4
	tmp = 0;
	//ft_printf("preparing output for exec[%d]\n", i);
	if ((i + 1) < execl->valid_cmds)
		close(fd[i + 1][0]);
	if (execl->chunk[i]->outfile != NULL) //1, outfile
	{
		//buff = NULL;
		//if (i != 0) //inpipe, infile, heredoc
		buff = empty_pipe(fd[i][0]);
		nfd = ft_calloc(2, sizeof(int));
		pid = fork();
		if (pid == 0)
		{
			if ((i + 1) < execl->valid_cmds)
				close(fd[i + 1][1]);
			if (buff)
				temp_pipe(nfd, pid, buff);
			if (execl->chunk[i]->append == 1) //append redir[i][1]
				tmp = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
			else // truncate
				tmp = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
			dup2(tmp, STDOUT_FILENO);
			close(tmp); //depois de dup, fecha-se
			execve(exec_str[i][0], exec_str[i], execl->my_envp);
		}
		wait(NULL);
		if ((i + 1) < execl->valid_cmds) //outfile inside pipeline
		{
			if (buff)
				temp_pipe(nfd, pid, buff);
			dup2(fd[i + 1][1], STDOUT_FILENO);
			close(fd[i + 1][1]);
			if (buff)
				free(buff);
			free(nfd);
			execve(exec_str[i][0], exec_str[i], execl->my_envp);
		}
		else
		{
			if (buff)
				free(buff);
			free(nfd);
			exit(0);
		}
	}
	else if ((i + 1) < execl->valid_cmds && execl->chunk[i]->outfile == NULL) //2, inside pipeline, non outfile
		dup2(fd[i + 1][1], STDOUT_FILENO);
	if ((i + 1) < execl->valid_cmds)
		close(fd[i + 1][1]);
}



















































/*
void	exec_output(t_execlist *execl, int **fd, int **redir, int i)
{
	//ft_printf("preparing output for exec[%d]\n", i);
	//if ((i + 1) < execl->valid_cmds)
		//close(fd[i + 1][0]);
	if (execl->chunk[i]->outfile != NULL) //1, outfile
	{
		//ft_printf("inside outfile [%d]\n", i);
		if (execl->chunk[i]->append == 1) //append
		{
			//ft_printf("outfile append output [%d]\n", i);
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
		}
		else // truncate
		{
			//ft_printf("outfile truncate output [%d]\n", i);
			redir[i][1] = open(execl->chunk[i]->outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
		}
		dup2(redir[i][1], STDOUT_FILENO);
		//ft_printf("dup2(redir[%d][1] = %d, %d)\n", i, redir[i][1], STDOUT_FILENO);
		close(redir[i][1]); //depois de dup, fecha-se
		if ((i + 1) < execl->cmd_nmb) //outfile inside pipeline
		{
			//ft_printf("next is pipeline output [%d]\n", i);
			close(fd[i + 1][1]);
			fd[i + 1][1] = redir[i][1];
			//ft_printf("do you get out?\n");
			return ;
		}
	}
	else if ((i + 1) < execl->valid_cmds && execl->chunk[i]->outfile == NULL) //2, inside pipeline, non outfile
	{
		//ft_printf("pipe pipeline output [%d]\n", i);
		//ft_printf("dup2(fd[%d][1] = %d, STDOUT_FILENO = %d);\n", (i + 1), fd[i + 1][1], STDOUT_FILENO);
		dup2(fd[i + 1][1], STDOUT_FILENO);
	}
	if ((i + 1) < execl->valid_cmds)
	{
		//ft_printf("last cmd [%d]\n", i);
		//close(fd[i + 1][1]);
		//ft_printf("In output, closed(fd[%d][1] = %d)\n", (i + 1), fd[i + 1][1]);
	}
	//ft_printf("last cmd || out of output [%d]\n", i);
}
*/