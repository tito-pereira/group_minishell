/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_action.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:39:10 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/21 15:58:24 by tibarbos         ###   ########.fr       */
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

void	write_inpipe(t_execlist *execl, char *str, int **fd, int i)
{
	int 	pid;
	int		infile;
	char	*shovel;

	//ft_printf("writing inpipe\n");
	pid = fork();
    if (pid == 0)
    {
		if ((i + 1) < execl->valid_cmds)
		{
			close(fd[i + 1][0]);
			close(fd[i + 1][1]);
		}
		close(fd[i][0]);
		////////////////////
		infile = open(str, O_RDONLY); //possivel erro de nao haver file
		shovel = get_next_line(infile);
		while (shovel != NULL)
		{
			write(fd[i][1], shovel, ft_strlen(shovel)); //talvez seja preciso '\n' depois
			free(shovel);
			shovel = get_next_line(infile);
		}
		close(infile);
		////////////////////
		close(fd[i][1]);
		exit(0);
	}
    wait(0);
}

void	exec_input(t_execlist *execl, int **fd, int i)
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
	//close(fd[i][1]); //fecha o pipe local (so escreve no proximo)
    if (execl->chunk[i]->heredoc == 0 && execl->chunk[i]->inpipe == 1
		&& execl->chunk[i]->infile != NULL) //1, normal infile
    {
		//ft_printf("infile input [%d]\n", i);
        //redir[i][0] = open(execl->chunk[i]->infile, O_RDONLY);
        //dup2(redir[i][0], STDIN_FILENO);
		//close(redir[i][0]); //depois de dup, fecha-se
		write_inpipe(execl, execl->chunk[i]->infile, fd, i);
		dup2(fd[i][0], STDIN_FILENO);
    }
	close(fd[i][1]); 
	if (i > 0) //resto, ate ao ultimo
	{
		//ft_printf("pipeline input [%d]\n", i);
        dup2(fd[i][0], STDIN_FILENO); //last pipe, fd[i - 1][0];
		//ft_printf("dup2(fd[%d][0] = %d, STDIN_FILENO = %d);\n", i, fd[i][0], STDIN_FILENO);
	}
	if (execl->chunk[i]->outfile == NULL)
	{
		printf("closing inpipe in [%d]\n", i);
    	close(fd[i][0]);
	}
	//porque ainda vou precisar desta ponta do pipe para ler redir
}

void	exec_output(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	//int		pid;
	//int		tmp;
	//char	*buff;
	//int		*nfd;

	//pid = -2; //ls > tmp1 | cat > tmp2 | cat > tmp3 | cat > tmp4
	//ft_printf("preparing output for exec[%d]\n", i);
	if ((i + 1) < execl->valid_cmds)
		close(fd[i + 1][0]);
	if (execl->chunk[i]->outfile != NULL) //1, outfile
	{
		ex_outfile(execl, fd, i, exec_str);
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