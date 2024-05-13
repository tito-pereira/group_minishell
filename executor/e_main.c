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

/////////////////////////

void	init_exec(t_execlist *execl, int **fd, int **redir)
{
	int i;
	//int	j;
	int	ret;
	
	i = -1;
	//ft_printf("Initializing the executor function.\n");
	while (++i < execl->valid_cmds)
	{
		ft_printf("i: %d < cmds: %d\n", i, execl->valid_cmds);
		redir[i] = ft_calloc(2, sizeof(int));
		fd[i] = ft_calloc(2, sizeof(int));
		/*j = -1;
		while (++j < 2)
		{
			ft_printf("j: %d\n", j);
			//redir[i][j] = -1;
			ft_printf("redir[%d][%d]: %d\n", i, j, redir[i][j]);
			//fd[i][j] = -1;
			ft_printf("fd[%d][%d]: %d\n", i, j, fd[i][j]);
		}*/
		ret = pipe(fd[i]);
		if (ret == 0)
		{
			ft_printf("pipe[%d] success\n", i);
			ft_printf("fd[%d][0]: %d\nfd[%d][1]: %d\n", i, fd[i][0], i, fd[i][1]);
		}
		else
			ft_printf("pipe[%d] failed\n", i);
	}
	//ft_printf("fd && redir initialized.\n");
}

void	end_exec(t_execlist *execl, int **fd, int **redir, char ***exec_str)
{
	int i;
	
	i = -1;
	ft_printf("Ending the executor function.\n");
	while (++i < execl->valid_cmds)
	{
		exec_str[i] = free_db_str(exec_str[i]);
		free(redir[i]);
		close(fd[i][0]);
		close(fd[i][1]);
		free(fd[i]);
	}
	free(redir);
	free(fd);
	free(exec_str);
	ft_printf("All frees done.\n");
}

////////////////////////////////

/*
vindo do step 6:

se for builtin: (+ 1)
- cmd_n_args vem inalterado
- chunk->path vem com o path para o meu executavel de builtins

se for terminal: (+ 0)
- cmd_n_args ja vem modificado com o path em primeiro lugar
*/

void	get_exec_str(t_execlist *execl, char ***exec_str)
{
	int	i;
	int	c;

	c = -1;
	ft_printf("Creating all the exec_str structs.\n");
	while (execl->chunk[++c] != NULL)
	{
		i = 0;
		ft_printf("Command[%d]\n", c);
		while (execl->chunk[c]->cmd_n_args[i] != NULL)
			i++;
		ft_printf("Size[%d]: %d\n", c, i);
		exec_str[c] = malloc((i + 1 + execl->chunk[c]->blt) * sizeof(char *));
		ft_printf("Malloc[%d]: %d\n", c, (i + 1 + execl->chunk[c]->blt));
		if (execl->chunk[c]->blt == 1)
		{
			exec_str[c][0] = ft_strdup(execl->chunk[c]->path);
			ft_printf("BUILTIN:\nexec_str[%d][0]:%s;\n", c, exec_str[c][0]);
		}
		i = -1;
		while (execl->chunk[c]->cmd_n_args[++i] != NULL)
		{
			exec_str[c][i + execl->chunk[c]->blt] = ft_strdup(execl->chunk[c]->cmd_n_args[i]);
			ft_printf("exec_str[%d][%d]:%s ", c, (i + execl->chunk[c]->blt), exec_str[c][i + execl->chunk[c]->blt]);
			ft_printf("= execl->chunk[%d]->cmd_n_args[%d]:%s;\n", c, i, execl->chunk[c]->cmd_n_args[i]);
		}
		exec_str[c][i + execl->chunk[c]->blt] = NULL;
		ft_printf("exec_str[%d][%d]:NULL;\n", c, (i + execl->chunk[c]->blt));
	}
	ft_printf("All the exec_str created.\n");
}

////////////////////////////////

int	exec_main(t_execlist *execl, int *exit_stt)
{
	int		**fd;
	int		**redir;
	char	***exec_str;

	//ft_printf("Inside the executor:\n");
	(void)exit_stt;
	fd = (int **)ft_calloc(execl->valid_cmds, sizeof(int *));
	redir = (int **)ft_calloc(execl->valid_cmds, sizeof(int *));
	exec_str = (char ***)ft_calloc(execl->valid_cmds, sizeof(char **));
	if (!fd || !redir || !exec_str)
	{
		if (fd)
			free(fd);
		if (redir)
			free(redir);
		if (exec_str)
			free(exec_str);
		return (0);
	}
	//ft_printf("All mallocs succesfull\n");
	init_exec(execl, fd, redir); // V
	get_exec_str(execl, exec_str); // V
	exec_loop(execl, fd, redir, exec_str); // X
	end_exec(execl, fd, redir, exec_str); // X
	return (1);
}

/*
inicializa tudo (exec_str, fd, redirs)
prep exec_str ()
real time exec:
- cria todos os pipes e atribui nos fds
- todas as exec_actions fecham em loop tudo o que devem
- sao feitas as dup2 ao mm tempo que se fecha
- usam o exec_str diretamente feito antes
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