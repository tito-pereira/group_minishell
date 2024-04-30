/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:38 by marvin            #+#    #+#             */
/*   Updated: 2024/05/01 00:41:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		last = exec_chunk(execl);
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

void	**get_exec_str(t_chunk *chunk, char ***exec_str)
{
	int	i;

	i = 0;
	if (chunk->blt == 1) //builtin
	{
		while (chunk->cmd_n_args[i] != NULL)
			i++;
		*exec_str = malloc((i + 2) * sizeof(char *));
		*exec_str[0] = ft_strdup(chunk->path);
		i = -1;
		while (chunk->cmd_n_args[++i] != NULL)
			*exec_str[i + 1] = ft_strdup(chunk->cmd_n_args[i]);
		*exec_str[i] = NULL;
	}
	else if (chunk->blt == 0) //terminal, cmd_n_args tradicional
		*exec_str = chunk->cmd_n_args;
	//ja vem formatado do step 5 e tudo
}

void	exec_chunk(t_execlist *execl, char *exec_str)
{
	int		*fd;
	int		*redir;
	int		pid;
	int		i;
	int		inherit;
	
	i = -1;
	fd_in_out = malloc(2 * sizeof(int));
	redir[0] = -1;
	redir[0] = -1;
	inherit = -1;
	while (execl->chunk[++i] != NULL)
	{
		get_exec_str(execl->chunk[i], &exec_str);
		fd = malloc (2 * sizeof(int));
		pipe(fd);
		pid = fork();
		if (pid == 0)
		{
			if (execl->chunk[i]->infile != NULL && execl->chunk[i]->inpipe == 1) //1, infile redir valido
			{
				redir[0] = open(infile, O_RDONLY);
				dup2(redir[0], STDIN_FILENO);
				close(redir[0]); //depois de dup, fecha-se
			}
			else if (i > 0) //resto, inpipe redir invalido, comando nº>1, recebe smp pipe
				dup2(fd[0], STDIN_FILENO);
			close (fd[0]); // se tiver que ser usado, é usado antes daqui
			//acho que se fecha o original depois da dup certo?
			// else, normal input sem dup
			//------------------//
			if (execl->chunk[i]->outfile != NULL) //1, outfile redir
			{
				if (execl->chunk[i]->append == 1)
					redir[1] = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
				else
					redir[1] = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
				if ((i + 1) < execl->cmd_nmd) //more commands after
					execl->chunk[i + 1]->inpfd = redir[1];
				dup2(redir[1], STDOUT_FILENO);
				close(redir[1]); //depois de dup, fecha-se
			}
			if ((n + 1) < execl->cmd_nmb) //2, next chunk redir
			{
				execl->chunk[i + 1]->inpfd = redir[1];
				if (execl->chunk[i]->outfile == NULL)
					redir[1] = (outra ponta do pipe);
				dup2(fd[1], STDOUT_FILENO);
			}
			// else, output normal sem dup
			execve(exec_str[0], exec_str, execl->my_envp);
		}
		wait(0);
		close(fd[1]); //para assumir a ponta de leitura do pipe
		//transporta o fd[0] para o proximo fork
		if ((i + 1 == execl->cmd_nmb)) //ultimo comando
			close(fd[0]);
		else
			inherit = fd[0];
		free(fd);
		if (execl->chunk[i]->blt == 1) //if builtin, dou malloc antes
			free_db(exec_str);
	}
	//free(fd);
}

/*
o pipe desta funçao é para ligar o output previo com o input seguinte
inpfd começa em -1 por default e pode ser alterado no loop
outpfd também

retorna a reading parte do pipe?

faço já uma exec_struct e mando o prototype para o execve?
ou faço condições?
*/

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