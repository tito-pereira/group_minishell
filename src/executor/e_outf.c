/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_outf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:25:54 by tibarbos          #+#    #+#             */
/*   Updated: 2024/06/23 08:45:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ex_redir_file(t_execlist *execl, int i, char *buff, int *nfd)
{
	int	tmp;
	int	n_file;

	tmp = 0;
	n_file = execl->chunk[i]->nmb_outf;
	if (buff)
		temp_pipe(nfd, buff);
	if (execl->chunk[i]->append == 1) //append
		tmp = open(execl->chunk[i]->outfiles[n_file], \
		O_RDWR | O_CREAT | O_APPEND, 0644);
	else // truncate
		tmp = open(execl->chunk[i]->outfiles[n_file], \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
	dup2(tmp, STDOUT_FILENO);
	close(tmp); //depois de dup, fecha-se
	//execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
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

void	ex_outfile(t_execlist *execl, int **fd, int i)
{
	char	*buff;
	int		*nfd;
	int 	pid;
	
	//printf("inside outfile in [%d]\n", i);
	buff = empty_pipe(fd[i][0], execl, i);
	//printf("buff:%s;\n", buff);
	nfd = ft_calloc(2, sizeof(int));
	pid = fork();
	if (pid == 0) //redir para outfile
	{
		if ((i + 1) < execl->valid_cmds)
			close(fd[i + 1][1]);
		ex_redir_file(execl, i, buff, nfd);
		ex_end(buff, nfd);
		return ;
	}
	wait(NULL);
	if ((i + 1) < execl->valid_cmds) //outfile inside pipeline
	{
		ex_redir_pipe(fd, i, buff, nfd);
		//execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
		ex_end(buff, nfd);
	}
}

/*
fora desta funcao, apenas vai haver isto
if ((i + 1) < execl->valid_cmds)
		close(fd[i + 1][1]);
antes de sair e ir para o exec
-> fora desta funcao faz logo return p fora dela, é uma saida direta

redir - fecho ali o fd no inicio do redir fork
pipe - faz dup e fecha a dup, execve fecha o original

(V) retirar execve
(V) cuidado c os dois processos a correr ao mm tempo ao sair
(V) garantir que ambos os casos entram no execve la fora (e nas builtins tmb ja agora)
() corrigir erro 1

desde que outfiles != NULL, entra aqui
portanto aqui entram todos de facto, agora é uma questao de gerir aqui dentro
- buff nulo
- redirs bloqueiam
if (buff)
	temp_pipe(nfd, buff);
(mantem se o dup2 / default STDIN vindo do exec_input)
- apenas se faz dup2 output


teoricamente nao deve dar problema
vao todos ao temp pipe, suponho eu para fazer dup2 do input, que depois voltaria as redirs, onde fazem dup2 output
e entrava logo no execve. logo, as temps recebem o buff para fazer dup2 de input suponho eu, para o nfd[].
o que eu tenho de mudar: caso haja buff, fazer isto. caso nao haja buff, nao acontece nada com o nfd e fica apenas
o default input vindo de cima (default STDIN) e retorna operavel mas sem realizar qualquer mudanca
*/


/*
escrevo o buff para nfd
de seguida, numa opcao ou noutra, faco execve direto
e depois fecho o buff e o nfd
o resto da exec_output apenas faz dup2
so aqui que eu faco execve direto
o que eu vou querer é:
ex buff (1 file) -> new nfd (2 files, 1 em cada processo)
- eliminar os execves aqui dentro;
- trazer o nfd[0] comigo para o execve fora aqui, dentro da dup2()
(como o fd se fecha sozinho, eu posso apenas fechar a dup na altura e nao
me preocupar mais)
- ajustar o path para fora da funcao porque vou ter 2 processos a correr em simultaneo:
o do pipe e o da redirection

(V) -> erro numero 1
eu so faco redirection de outfile quando tenho algo a entrar
<infile command >outfile
command | command >outfile
e se for este caso, sera que eu considero?
echo hi >out.txt

(V) -> erro numero 2
faco logo execve aqui dentro, o que exclui todos os builtins que so
correm depois de sair daqui e nao sao execve
. tirar o execve
. deixar o processo continuar a correr para fora do exec_output
. deixar o processo entrar no execve dentro da exec_loop ou na builtin

(V) -> erro numero 3
ao criar o buff, eu esvazio o infile, tornando o dup2 nulo e usando um execve vazio
(mas esse erro nao deve existir porque eu duplico o file e trago um nfd[0] novo comigo)
*/