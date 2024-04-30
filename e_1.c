/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:38 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 18:06:42 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
c = chunk iteration;
pid, fd, pipe, fork = execve e conexao de pipes

diferentes condicoes pipe flags e redirs para definir os dups
*/

int	exec_chunk(t_execlist *execl)
{
	int	*fd;
	int	pid;
	int	c;

	c = execl.current;
	fd = malloc (2 * sizeof(int));
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (inpipe == 1) // inpipe valido
			dup2(execl.chunk[c].inpfd, STDIN_FILENO);
		else if (execl->chunk[c]->inpipe == 0 && infile != NULL)
			dup2(open(execl.chunk[c].infile, O_RDONLY), STDIN_FILENO);
		if (outfile != NULL)
			dup2(open(execl.chunk[c].outfile, O_RDONLY), STDOUT_FILENO);
		if (outpipe != NULL)
			execl.chunk[c + 1].inpfd = execl.chunk[c].outpfd; //a ser usado no comando seguinte
		execve(execl.chunk[c].cmd_n_args[0], execl.chunk[c].cmd_n_args, execl->my_envp);
	}
	wait(0);
	close(fd[1]);
	return (fd[0]);
}

/*
pega no fd do ultimo pipe fd e copia tudo (getnextline) para o STDOUT
*/
void	last_output(int last)
{
	char	*ret;
	int		sig;

	sig = 1;
	while (sig == 1)
	{
		ret = get_next_line(last);
		if (ret != NULL)
		{
			ft_printf("%s\n", ret);
			free(ret);
		}
		else
			sig = 0;
	}
	close(last);
}
/*
retornar o errno apenas no fim, nos passos intermédios
nao é realmente necessário

o ultimo comando tem que executar para o terminal
por isso tenho que ligar o pipe final e copiar o output todo
para o STDOUT
tou a pensar usar get_next_line(pipe->STDOUT)
*/

int	the_executor(t_execlist *execl, int *error_stt)
{
	int	i;
	int	last;

	if (execl->cmd_nmb != (execl->pipe_nmb + 1))
	{
		perror("Number of pipes and commands is not compatible");
		*error_stt = 1;
		return (0);
	}
	i = -1;
	while (execl->chunk[++i] != NULL) //chunk loop e executar cada chunk
	{
		last = exec_chunk(execl, i);
		//execl.current++;
	}
	execl.erno = errno; //errno do ultimo comando antes de ser contaminado c a getnextline
	last_output(last);
	//return (1);
}

/*
typedef struct s_chunk {
	char	*infile; // (2) redir_checker
	int		heredoc; // (2) redir_checker
	char	*delimiter; // (2) redir_checker
	char	*outfile; // (2) redir_checker
	int		append; // (2) redir_checker
	char	*og; // (1) parse_execl
	char	**cmd_n_args; // (4) arg_separator
	char	*path; // (5) arg_id
	int		inpipe; // (5) arg_id
	int		inpfd; // executor
	int		outpipe; //acho que já não é necessário
	int		outpfd; // executor
	int		blt; // (5) arg_id
}	t_chunk;

execve struct maker
if (chunk[i]->blt == 0)
	cmd_n_args normal
else if (chunk[i]->blt == 1)
{
	struct[0] = path;
	resto = copy;
}

INPUT REDIR:
inpipe - apenas o primeiro pipevalido

OUTPUT REDIR
outpipe = quando o comando nao e o ultimo, ou quando ha outfile
guardar fd final (outpfd)
if outfile -> dup2 outfile e retornar outfile fd (outpfd)

if (append)
	int fd = open("filename.txt", O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0644);
(tem a ver com as flags do open)

(blt == 0)
execve(execl->chunk[c]->cmd_n_args[0], execl->chunk[c]->cmd_n_args, execl->my_envp);
(blt == 1)
execve(execl->chunk[c]->path, execl->chunk[c]->cmd_n_args, execl->my_envp);
*/

int	exec_chunk(t_execlist *execl, int n)
{
	int	*fd;
	int	fd_in;
	int	fd_out;
	int	pid;
	
	fd = malloc (2 * sizeof(int));
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		if (infile && inpipe == 1)
		{
			fd_in = open(O_RDONLY);
			dup2(fd_in, STDIN_FILENO);
		}
		else if (dentro da pipeline)
			dup2(fd_in, STDIN_FILENO)
		//////
		dup2(fd[1], STDOUT_FILENO);
		if (inpipe == 1) // inpipe valido
			dup2(execl.chunk[c].inpfd, STDIN_FILENO);
		else if (execl->chunk[c]->inpipe == 0 && infile != NULL)
			dup2(open(execl.chunk[c].infile, O_RDONLY), STDIN_FILENO);
		if (outfile != NULL)
			dup2(open(execl.chunk[c].outfile, O_RDONLY), STDOUT_FILENO);
		if (outpipe != NULL)
			execl.chunk[c + 1].inpfd = execl.chunk[c].outpfd; //a ser usado no comando seguinte
		execve(execl.chunk[c].cmd_n_args[0], execl.chunk[c].cmd_n_args, execl->my_envp);
	}
	wait(0);
	close(fd[1]);
	return (fd[0]);
}

/*
/------------------------------------------------------------------/
execve()
char *PATH, char **ARGS, char **execl->my_envp

av[1] = (char *)"./builtins/builtfunct";
av[2] = (char *)*error_stt; (atualizar o error status, se for, p 126)
av[3] = (char *)execl->chunk[i]->cmd_n_args (cmd_n_args ponto 4)

execl->my_envp; //step 1
execl->path; //step 5
cmd_n_args mantém-se do ponto 4

Exit status 2: Misuse of shell builtins (e.g., incorrect usage of a command).
(built in failed)
Exit status 126: Permission problem or command is not executable.
(executor failed)
(after cada execve)

(step 5 feito, falta testing)
.mudar as t_mini para um int *error
.fazer executor
*/