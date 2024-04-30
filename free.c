/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 17:48:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_db(char **str)
{
	int	i;

	i = -1;
	while (str[++i] != NULL)
	{
		free (str[i]);
		str[i] = NULL;
	}
	free(str);
}

void	free_chunk(t_chunk *chunk)
{
	if (chunk->infile != NULL)
		free(chunk->infile);
	if (chunk->delimiter != NULL)
		free(chunk->delimiter);
	if (chunk->outfile != NULL)
		free(chunk->outfile);
	if (chunk->infile != NULL)
		free(chunk->infile);
	if (chunk->cmd_n_args != NULL)
		free_db(chunk->cmd_n_args);
	free(chunk);
}

void	free_exec(t_execlist *exec)
{
	int	i;

	i = -1;
	while (exec->chunk[++i] != NULL)
		free_chunk(exec->chunk[i]);
	free(exec);
}

/*
preparar o free_exec para todas as eventualidades
desde completamente cheio e inicializado,
ate completamente vazio e inicializado
*/

/*
typedef struct s_chunk {
	char	*infile;
	int		heredoc;
	char	*delimiter;
	char	*outfile;
	int		append; //0 se não (truncate), 1 se sim (append)
	char	*og;
	char	**cmd_n_args;
	int		inpipe; //1 se for precedido de um pipe
	int		inpfd; //se sim, o seu fd, default -1
	int		outpipe; //1 se for redirecionado para um pipe
	int		outpfd; //se sim, o seu fd, default -1
}	t_chunk;

typedef struct s_execlist {
	t_chunk		**chunk;
	int			cmd_nmb;
	int			pipe_nmb;
	int			current; //numero de comando, default 1
	int			erno;
}	t_execlist;
*/