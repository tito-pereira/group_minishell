/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:50:43 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/06 17:40:56 by tibarbos         ###   ########.fr       */
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
	ft_printf("checking for redirs to remove\n");
	while (chunk->cmd_n_args[++i] != NULL)
	{
		if (chunk->cmd_n_args[i][0] == '>'
			|| chunk->cmd_n_args[i][0] == '<')
			{
				ft_printf("redir found in arg %d\n", i);
				return (i);
			}
	}
	ft_printf("no redirection found to remove\n");
	return (-1);
}

void	remove_redir(t_chunk *chunk, int red)
{
	char	**new;
	int		i;
	int		c;

	i = 0;
	ft_printf("lets get this bad boy out\n");
	while (chunk->cmd_n_args[i] != NULL)
		i++;
	ft_printf("total args is [%d]\n", (i - 1));
	ft_printf("gonna remove [%d] and [%d]\n", red, (red + 1));
	new = malloc((i - 1) * sizeof(char *));
	i = -1;
	c = 0;
	while (++i != red)
	{
		new[c++] = chunk->cmd_n_args[i];
		ft_printf("new[%d] = cmd[%d] = '%s'\n", (c - 1), i, new[c - 1]);
	}
	if (i == red)
		i++;
	while (chunk->cmd_n_args[++i] != NULL)
	{
		new[c++] = chunk->cmd_n_args[i];
		ft_printf("new[%d] = cmd[%d] = '%s'\n", (c - 1), i, new[c - 1]);
	}
	ft_printf("new[%d] will be NULL\n", c);
	new[c] = NULL;
	if (chunk->cmd_n_args[red])
		free(chunk->cmd_n_args[red]);
	if (chunk->cmd_n_args[red])
		free(chunk->cmd_n_args[red + 1]);
	chunk->cmd_n_args = new;
	ft_printf("removal finished\n");
}

/*
nao esquecer a alocacao de memoria do p_4 para futuros testes
no entanto

tenho que colocar o i no sitio correto aqui
*/

int	rmv_redirs(t_execlist *execl, int *exit_stt)
{
	int	i;
	int	red;

	i = -1;
	red = 0;
	(void)exit_stt;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (5): rmv_redirs;\n");
	while (execl->chunk[++i] != NULL)
	{
		ft_printf("chunk[%d]\n", i);
		while (red != -1)
		{
			ft_printf("loop\n");
			red = verify_redir(execl->chunk[i]);
			if (red != -1)
				remove_redir(execl->chunk[i], red);
		}
	}
	return (1);
}
