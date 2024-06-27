/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_outf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:25:54 by tibarbos          #+#    #+#             */
/*   Updated: 2024/06/27 20:08:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ex_end(char *buff, int *nfd)
{
	if (buff)
		free(buff);
	if (nfd)
		free(nfd);
	buff = NULL;
	nfd = NULL;
	//exit(0);
}

/*
bruh nao posso ter um exit aqui wtf como é que eu estava sequer a funcionar com isto?
sera que eram vestigios de quando tinha os execve?
*/

void	ex_redir_file(t_execlist *execl, int i, char *buff, int *nfd, \
	char ***exec_str)
{
	int	tmp;
	int	n_file;

	tmp = 0;
	n_file = execl->chunk[i]->nmb_outf;
	if (buff)
		temp_pipe(nfd, buff); //cria o artifical infile e dup2 input
	ex_end(buff, nfd);
	if (execl->chunk[i]->append == 1) //append
		tmp = open(execl->chunk[i]->outfiles[n_file], \
		O_RDWR | O_CREAT | O_APPEND, 0644);
	else // truncate
		tmp = open(execl->chunk[i]->outfiles[n_file], \
		O_RDWR | O_CREAT | O_TRUNC, 0644);
	dup2(tmp, STDOUT_FILENO);
	close(tmp); //depois de dup, fecha-se
	execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
}

/*
depois do temp pipe, ja fiz dup2, já escrevi o buff, posso
dar free logo ali there and then (antes do execve)
e depois ainda fica a faltar o ultimo free acho eu? nope, só o ex_end

implementar o execve aqui only if X BLT
*/

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

/*
tmp pipe
faz fork, escreve buff para o pipe, e faz dup2 de input para esse pipe
nfd sera o o int* desse novo pipe
*/

/*
if buff, escreve o buff para o nfd, e faz dup2 do fd para output
qual o sentido disto?
*/

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
	if (pid == 0) //redir para outfile, feito em fork (NAO CONTA), a nao ser que tambem faça exec
	{
		if ((i + 1) < execl->valid_cmds) //sera que é valid_cmds ou cmd_nmb?
			close(fd[i + 1][1]);
		if (execl->chunk[i]->blt == 0)
			ex_redir_file(execl, i, buff, nfd, exec_str);
		else if (execl->chunk[i]->blt == 1)
		{
			ex_end(buff, nfd);
			exit(0); //sera que o chunk[i] é o numero correto?
		}
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
-> redir outfile COM EXEC (only if X BLT)
-> redir pipe mantém se apenas com dup2 para ambos
-> trazer o fd outfile cá para fora para o BLT

o ultimo redir pipe honestamente também nao preciso para nada
para os builtins
se for builtin acho que posso simplesmente sair desta funcao maybe?
escuso de estar

temp pipe
como eu esvaziei o original infile, crio um pipe artificial só
para simular um infile e ter um actual file descriptor para usar como
dup2

e se eu, ao executar 2 vezes os BLT, também esgotar o infile?
o unico que recebe input é o echo certo?
opa posso se calhar guardar o buff na execl idk... os outros nenhum deles
precisa de infile, apenas args e outfile.. mas vou me lixar com
echo <f1 | echo <f2
qualquer comando | echo <f1
wait nao isso sao chunks diferentes..
é so com
echo <f1 | cat, algum destes nao vai ter output (f1, terminal)
retiro o que disse, echo nao aceita infile input, NENHUM aceita tho
nao preciso entao de me preocupar com isso
se calhar até testo os comandos depois e caso aceitem input eu venho aqui
especificamente saltar isso à frente if (blt == 1)


----------------------

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