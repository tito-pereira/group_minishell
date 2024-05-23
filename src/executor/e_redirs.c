/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:29:03 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/23 15:27:00 by tibarbos         ###   ########.fr       */
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
			if (chunk->here_dcs[i] == 0)
			{
				tmp = open(chunk->infiles[i], O_RDONLY | O_CREAT, 0644);
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
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
			else if (chunk->app_dcs[i] == 1)
				tmp = open(chunk->outfiles[i], O_RDWR | O_CREAT | O_APPEND, 0644);
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
		if (execl->chunk[c]->outfiles != NULL)
			open_all_outfs(execl->chunk[c], execl->exit_stt);
	}
}

/*
-> mudar aquele codigo de permissoes?
nao é suposto ver as permissoes do file?
se calhar eu recolho aqui ao tentar abrir com certas permissoes

-> mudar error status
*/