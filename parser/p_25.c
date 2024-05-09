/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_25.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:12:29 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/09 13:36:03 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
(5) - remove redirections from the valid commands && arguments char **

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

void	remove_redir(t_chunk *chunk)
{
	int	i;
	int	a;
	int	b;

	i = -1;
	while (chunk->og[++i] != '\0')
	{
		if (chunk->og[i] == '<' || chunk->og[i] == '>');
		{
			a = i;
			while ('>')
				i++;
			while (' ')
				i++;
			while ('asdsa')
				i++;
			b = i;
			strdup (first)
		}
	}
}

/*
copiar ideias da p_3 incluindo chunk conditions etc
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