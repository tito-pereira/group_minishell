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

void	exec_chunk(t_execlist *execl, char **exec_str, int *error_stt)
{
	int		pid;
	int		i;
	
	i = -1;
	ft_printf("inside exec_chunk\n");
	while (execl->chunk[++i] != NULL && *error_stt != 126)
	{
		pipe(fd);
		pid[i] = fork();
		if (getpid())
		{
			input_options(execl, fd, redir, i);
			output_options(execl, fd, redir, i);
		}
	}
}

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
*/