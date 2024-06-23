/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2b.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 22:44:10 by marvin            #+#    #+#             */
/*   Updated: 2024/05/22 22:44:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
neste file aqui tenho as tais funçoes de update
os nomes das file redirections, que recebem os parametros
funcoes sao chamadas no file 2a quando de facto se confirma
uma presenca de redirection e se recolhe o nome dessa tal redirection
*/

char	**add_char_p(char **old, char *n_str)
{
	char	**new;
	int		i;

	i = 0;
	while (old[i] != NULL)
		i++;
	new = (char **)ft_calloc((i + 2), sizeof(char *));
	i = -1;
	while (old[++i] != NULL)
		new[i] = ft_strdup(old[i]);
	new[++i] = n_str; //ou strdup ainda n sei
	new[i] = NULL;
	free_db_char(old);
	return (new);
}

int	*add_int_p(int *old, int flag)
{
	int		*new;
	int		i;

	i = 0;
	while (old[i] != -1)
		i++;
	new = (int *)ft_calloc((i + 2), sizeof(int));
	i = -1;
	while (old[++i] != -1)
		new[i] = old[i];
	new[++i] = flag;
	new[i] = -1;
	free(old);
	return (new);
}

// chunk, in_out, flag, new
void	update_char_p(char ***in_or_out, char *n_str, int *c)
{
	if (*c == -1)
	{
		*in_or_out = (char **)ft_calloc(2, sizeof(char *));
		(*in_or_out)[0] = n_str;
		(*in_or_out)[1] = NULL;
		(*c)++;
	}
	else
	{
		*in_or_out = add_char_p(in_or_out, n_str);
		(*c)++;
	}
}

void	update_int_p(int **in_or_out, int flag, int c)
{
	if (c == 0)
	{
		*in_or_out = (int *)ft_calloc(1, sizeof(int));
		(*in_or_out)[0] = flag;
		(*in_or_out)[1] = -1;
	}
	else
		*in_or_out = add_int_p(in_or_out, flag);
}

/*
infile=0, outfile=1, heredoc && append == 1, nao heredoc && trunc == 0
*/

void	updt_rdr_lst(t_chunk *chunk, int in_out, int flag, char *n_str)
{
	if (in_out == 0 && flag == 0)
	{
		update_char_p(&(chunk->infiles), n_str, &(chunk->nmb_inf));
		update_int_p(&(chunk->here_dcs), flag, chunk->nmb_inf);
	}
	else if (in_out == 1 && flag == 0)
	{
		update_char_p(&(chunk->outfiles), n_str, &(chunk->nmb_outf));
		update_int_p(&(chunk->outfiles), flag, chunk->nmb_outf);
	}
}

/*
here_file nao preciso de guardar entao posso apenas
estar sempre a atualizar
vou atualizando é a flag heredoc.
quando há normal input, mete a flag heredoc a 0.
quando há heredoc, mete a flag a 1.
o ultimo a mexer na flag é o válido e é o valor
que fica

. infiles, keep adding
. heredoc = here_dc[nmb_inf]

. outfiles, keeps adding
. append, last one changes

ate aqui, acho que é assim
mas e quando eu tiver que realmente abrir os ficheiros, seja
trunc ou append, é so abrir e deixar em branco? ou realmente faco algo?
*/