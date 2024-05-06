/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:50:43 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/06 15:19:54 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
recebo o cmd_n_args dentro de cada chunk

acrescentar um novo step p_novo, pré ou pos p_4
se calhar pos p_4 da mais jeito
ve os args todos
ve str[0] == '<' || str[0] == '>'
alona novo char ** c tamanho - 2
retira esse arg e o proximo
free na antiga e retorna a nova char *

e se tiver multiplas redirections, vou ter que retirar tudo

red vai ser um indice
*/

int	verify_redir(t_chunk *chunk)
{
	int	i;

	i = -1;
	while (chunk->cmd_n_args[++i] != NULL)
	{
		if (chunk->cmd_n_args[i][0] == '>'
			|| chunk->cmd_n_args[i][0] == '<')
			return (i);
	}
	return (-1);
}

void	remove_redir(t_chunk *chunk, int red)
{
	char	**new;
	int		i;

	i = 0;
	while (chunk->cmd_n_args[i] != NULL)
		i++;
	new = malloc((i - 1) * sizeof(char *));
	i = 0;
	while (i != red)
		new[i] = chunk->cmd_n_args[i];
	if (i == red)
		i += 2;
	while (chunk->cmd_n_args[i] != NULL)
		new[i] = chunk->cmd_n_args[i];
	new[i] = NULL;
	free(chunk->cmd_n_args[red]);
	free(chunk->cmd_n_args[red + 1]);
	chunk->cmd_n_args = new;
}

int	rmv_redirs(t_execlist *execl, int *exit_stt)
{
	int	i;
	int	red;

	i = -1;
	red = 0;
	while (execl->chunk[++i] != NULL)
	{
		while (red != -1)
		{
			red = verify_redir(execl->chunk[i]);
			if (red != 1)
				remove_redir(execl->chunk[i], red);
		}
	}
	return (1);
}
