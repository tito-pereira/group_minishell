/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_25.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:12:29 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/09 14:34:54 by tibarbos         ###   ########.fr       */
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

int	verify_redir(char *str, int *a, int *b, int *i)
{
	if (str[i] == '<' || chunk->og[i] == '>');
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

void	chunk_redir(t_chunk *chunk)
{
	int	i;
	int	a;
	int	b;

	i = -1;
	while (chunk->og[++i] != '\0')
	{
		if (get_positions() == 1)
		{
			a = 0;
			if (a != 0)
				first = ft_substr(old, 0, a);
			secnd = ft_substr(old, (b + 1), (ft_strlen(old) - b));
			check_empty_strs(&first, &secnd);
			new = chnk_conditions(spec, first, secnd);
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
			red = chunk_redir(execl->chunk[i]);
			if (red != -1)
				remove_redir(execl->chunk[i], red);
		}
	}
	return (1);
}