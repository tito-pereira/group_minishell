/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:29:03 by tibarbos          #+#    #+#             */
/*   Updated: 2024/06/23 21:37:36 by marvin           ###   ########.fr       */
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

void	open_all_infs(t_chunk *chunk, int *exit)
{
	int	i;
	int	tmp;
	int	nfile;

	i = -1;
	nfile = chunk->nmb_inf;
	if (chunk->infiles != NULL)
	{
		while (++i <= nfile)
		{
			if (chunk->here_dcs[i] == 0) //nao heredoc
			{
				tmp = open(chunk->infiles[i], O_RDONLY); //| O_CREAT
				if (tmp == -1)
					*exit = 1; //verificar os erros possiveis
				close(tmp);
			} //so os nao heredoc abrem files, os heredoc passam à frente
		}
	}
}

void	open_all_outfs(t_chunk *chunk, int *exit)
{
	int	i;
	int	tmp;
	int	nfile;
	
	i = -1;
	nfile = chunk->nmb_outf;
	if (chunk->outfiles != NULL)
	{
		while (++i <= nfile)
		{
			if (chunk->app_dcs[i] == 0)
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_TRUNC);
			else if (chunk->app_dcs[i] == 1)
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_APPEND);
			if (tmp == -1)
				*exit = 1; //verificar os erros possiveis
			close(tmp);
		}
	}
}

void	open_all_redirs(t_execlist *execl)
{
	int	c;

	c = -1;
	while (execl->chunk[++c] != NULL)
	{
		if (execl->chunk[c]->infiles != NULL)
			open_all_infs(execl->chunk[c], execl->exit_stt);
		if (execl->chunk[c]->outfiles != NULL && execl->exit_stt == 0)
			open_all_outfs(execl->chunk[c], execl->exit_stt);
	}
}

/*

-> mudar error status
-> introduzir error messages

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

- open all redirs
mudar os nomes das variaveis no executor (na action ja estava mudado afinal)
- mudar no parser p_2b
*/