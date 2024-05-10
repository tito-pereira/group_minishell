/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_old.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 11:32:48 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/10 11:32:48 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	input_options(t_execlist *execl, int **fd, int **redir, int i)
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

void	output_options(t_execlist *execl, int **fd, int **redir, int i)
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

void	process_end(t_execlist *execl, char **exec_str, int **fd, int i)
{
	wait(0);
	close(fd[i][1]); //para assumir a ponta de leitura do pipe
	if ((i + 1) == execl->cmd_nmb) //ultimo comando
		close(fd[i][0]);
	else
		execl->chunk[i + 1]->inpfd = fd[i][0]; //override ao outfile inside pipeline
	free(fd);
	if (execl->chunk[i]->blt == 1) //if builtin, dou malloc antes
		free_db(exec_str); // tirar isto
}

/////////////////////////
void	prep_loop()
{
	while (execl->chunk[++i] != NULL) //&& *error_stt != 126)
	{
		input_options(execl, fd, redir, i);
		output_options(execl, fd, redir, i);
	}
}

void	exec_chunk(t_execlist *execl, char **exec_str, int *error_stt)
{
	int		*pid;
	int		**fd;
	int		*redir;
	char	***exec_str;
	int		i;
	
	pid = (int *)malloc(execl->valid_cmds * sizeof(int));
	fd = (int *)malloc(execl->valid_cmds * sizeof(int));
	redir = (int *)malloc((execl->valid_cmds * 2) * sizeof(int));
	exec_str = (char ***)malloc(execl->valid_cmds * sizeof(char **));
	i = -1;
	// initialize all the values
	prep_loop();
	exec_loop();
	// free all the values
}
/*
prep_input && prep_output (open/close)
exec_input && exec_output (dup2/execve)
ou
tudo junto?
*/

/*
execl->valid_cmds;

int	*pid = (int *)malloc(execl->valid_cmds * sizeof(int));
int	**fd = (int *)malloc(execl->valid_cmds * sizeof(int));
int	*redir = (int *)malloc((execl->valid_cmds * 2) * sizeof(int));
char	***exec_str = (char ***)malloc(execl->valid_cmds * sizeof(char **));
...caso seja preciso inicializar...
int i;
i = -1;
while (++i < (execl->valid_cmds * 2))
	redir[i] = -1;
int	j;
i = -1;
while (++i < execl->valid_cmds)
{
	j = -1;
	while (++j < 2)
		fd[i][j] = -1;
}
...................................

tenho que reaproveitar as ideias do executor original, o loop, para fazer um
tratamento previo onde defino, em cada chunk,

nao preocupar com frees, apenas, no fim de tudo, dou free aos tres:
pid = (int *)malloc;
fd = (int **)malloc;
redir = (int *)malloc;
exec_str = (char ***)malloc;
-> loop de atribuicao de dups e pipes;
-> correr em simultaneo todos os processos;
free(pid);
free(fd);
free(redir);
free(exec_str);

nao tenho acesso ao pid. so tenho pid == 0 em forks e waits
crio 1, passo 0;
i++;
if (i < execl->valid-cmds)
{
	create another
	if (pid = 0)
	{
		new_
	}
}
if (i < execl->valid-cmds)
	wait(0);

read, get_next_line, dup2, tudo o que for ler de pipes
desde que a ponta de escrita esteja vazia, esperam pelo fim da escrita

para alem disso, comandos tipo ls que nao precisem de input, saltam essa
parte e simplesmente correm

resumindo, esta implementacao em si ja vai resultar porque os comandos ja tem
esses mecanismos incluidos
also, depois de executar, o pipe fecha sozinho, por isso nem tenho q me preocupar
em fechar a ponta de escrita, ele executa e fecha e espera sozinho, top
*/

/*
------------------------------------------------
aquela parte esta feita, falta agora outra: open/closes de pipes e files

redirs - in/out files (redirs acho que se fecham sozinhos)
fds - pipes
ja sei
abro os pipes todos no main process previamente, tal como as redirs
e depois dentro de cada processo fecho os relativos in && out pipes para os "atribuir"
ou
faco uma iteracao a fechar todos os outros pipes que nao uso
ou
em cada iteracao dentro de cada fork individual em cada processo, crio o pipe aí

pelos vistos, se fechares uma ponta de um pipe, nao estas a automaticamente atribui lo
para escrever, isso so ocorre com quem escreve primeiro
*/