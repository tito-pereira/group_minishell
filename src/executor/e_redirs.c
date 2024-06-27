/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:29:03 by tibarbos          #+#    #+#             */
/*   Updated: 2024/06/27 20:02:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
typedef struct s_chunk {
	char	**infiles; //
	int		nmb_inf; //
	int		*here_dcs; //
	int		heredoc; // --- //
	char	*here_file; // --- //
	char	*delimiter;
	char	**outfiles; //
	int		nmb_outf; //
	int		append;
	int		*app_dcs; //
	char	*og;
	char	**cmd_n_args;
	char	*path; // --- //
	int		inpipe;
	int		inpfd; // --- //
	int		outpipe; // --- //
	int		outpfd; // --- //
	int		blt;
}	t_chunk;
*/

int	open_all_infs(t_chunk *chunk, int *exit)
{
	int	i;
	int	tmp;
	int	nfile;

	i = -1;
	nfile = chunk->nmb_inf;
	tmp = 0;
	(void)exit;
	//printf("nfile is %d\n", nfile);
	if (chunk->infiles)
	{
		while (++i <= nfile)
		{
			//printf("--- loop %d ---\n", i);
			if (chunk->here_dcs[i] == 0) //nao heredoc
			{
				//printf("will open now\n");
				//printf("chunk->infiles[%d]:%s\n", i, chunk->infiles[i]);
				tmp = open(chunk->infiles[i], O_RDONLY); //| O_CREAT
				//printf("opened in %d, tmp is %d\n", i, tmp);
				if (tmp == -1)
				{
					return (-1);
					//printf("opening error in infile[%d]", i);
					//printf("error = %d\n", *exit);
					//*exit = 1; //verificar os erros possiveis
				}
				//printf("will try to close tmp(%d) now\n", tmp);
				if (tmp != -1)
				{
					close(tmp);
					//printf("closed in %d with tmp(%d)\n", i, tmp);
				}
			} //so os nao heredoc abrem files, os heredoc passam à frente
		}
	}
	return (0);
}

/*
ok o erro foi no exit
ate mesmo na main open_all function, um simples print com o
exit_stt causa erro
*/

int	open_all_outfs(t_chunk *chunk, int *exit)
{
	int	i;
	int	tmp;
	int	nfile;
	
	i = -1;
	(void)exit;
	printf("opening all outfiles, %d in total\n", chunk->nmb_outf);
	nfile = chunk->nmb_outf;
	if (chunk->outfiles != NULL)
	{
		while (++i <= nfile)
		{
			if (chunk->app_dcs[i] == 0)
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
			else if (chunk->app_dcs[i] == 1)
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_APPEND, 0644);
			if (tmp == -1)
			{
				return (-1);
				//*exit = 1; //verificar os erros possiveis
			}
			printf("will try to close tmp(%d) now\n", tmp);
			if (tmp != -1)
			{
				close(tmp);
				printf("closed in %d with tmp(%d)\n", i, tmp);
			}
		}
	}
	return (0);
}

void	open_all_redirs(t_execlist *execl)
{
	int	c;

	c = -1;
	execl->exit_stt = 0;
	printf("--- INSIDE REDIR OPENING ---\n");
	for (int i = 0; execl->chunk[i] != NULL; i++)
		support_print(execl, i);
	while (execl->chunk[++c])
	{
		printf("inside loop\n");
		if (execl->chunk[c]->infiles == NULL)
			printf("infiles is NULL\n");
		if (execl->chunk[c]->infiles != NULL)
		{
			printf("inside infile loop\n");
			open_all_infs(execl->chunk[c], execl->exit_stt);
		}
		//printf("exit_stt is %d after infiles\n", *(execl->exit_stt));
		if (execl->chunk[c]->outfiles != NULL && execl->exit_stt == 0)
		{
			printf("inside outfile loop\n");
			open_all_outfs(execl->chunk[c], execl->exit_stt);
		}
	}
	printf("--- OUTSIDE REDIR OPENING ---\n");
}

/*
void	support_print(t_execlist *execl, int c)

-> mudar error status
-> introduzir error messages

sera que vou ter que separar entre file not found ou
nao tem permissoes?
sim.. sim vou
open retorna sempre -1 mas distingo através do errno, e consoante isso,
depois atualizo o exit_stt

<1 <2 <3
bash file 1 not found (apenas tenta abrir os files, nao os cria)
se eu nao tiver permissoes, nao consigo abrir
entao talvez retirar aquele numero ali das permissoes
é suposto eu retornar error messages por isso nao posso forçar permissions

heredocs?
sei que espera todos eles serem introduzidos ok
posso guarda-los na mesma
so abre caso seja non heredoc
de resto usa isso como input normal

>1 >2 >3 (caso nao existam, cria os)
funciona tanto para > como para >>

mensagem de erro é so do primeiro comando
NAO CRIA MAIS
caso haja um erro de file, nao cria os seguintes nem mesmo dentro do proprio chunk
cada chunk vai fazer esta verificacao

e dentro de pipes? como é suposto funcionar?
okok
todos os pipes criam as pastas, só por um nao funcionar nao interrompe os outros
onde ponho isto?
dentro de cada chunk, por isso tera de ser na loop? action?

mudar os nomes das variaveis no executor (na action ja estava mudado afinal)
*/