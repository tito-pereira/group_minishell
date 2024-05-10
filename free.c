/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/05/11 00:37:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str(char **str)
{
	free(*str);
	*str = NULL;
}

void	free_db_str(char ***str)
{
	int	i;

	i = -1;
	while (str[0][++i] != NULL)
		free_str(&str[0][i]);
	free(*str);
	*str = NULL;
}

void	free_chunk(t_chunk **chunk)
{
	if ((*chunk)->infile != NULL)
		free((*chunk)->infile);
	if ((*chunk)->delimiter != NULL)
		free((*chunk)->delimiter);
	if ((*chunk)->outfile != NULL)
		free((*chunk)->outfile);
	if ((*chunk)->cmd_n_args != NULL)
		free_db_str(&(*chunk)->cmd_n_args);
	free(*chunk);
	(*chunk)->infile = NULL;
	(*chunk)->outfile = NULL;
	(*chunk)->delimiter = NULL;
	*chunk = NULL;
}

void	free_exec(t_execlist **exec)
{
	int	i;

	i = -1;
	while ((*exec)->chunk[++i] != NULL)
		free_chunk(&((*exec)->chunk[i]));
	free(*exec);
	*exec = NULL;
}

char	*free_ptr(char *ptr)
{
	free(ptr);
	ptr = NULL;
	return (NULL);
}

char	**free_mat(char **mat)
{
	size_t	i;

	i = 0;
	while (mat && mat[i])
	{
		mat[i] = free_ptr(mat[i]);
		i++;
	}
	free (mat);
	mat = NULL;
	return (NULL);
}

/*
preparar o free_exec para todas as eventualidades
desde completamente cheio e inicializado,
ate completamente vazio e inicializado
*/