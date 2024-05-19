/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/05/19 15:35:36 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_str(char *str)
{
	free(str);
	return (NULL);
}

char	**free_db_str(char **str)
{
	int	i;

	i = -1;
	while (str[++i] != NULL)
		free_str(str[i]);
	free(str);
	return (NULL);
}

void	free_chunk(t_chunk *chunk)
{
	if (chunk->infile != NULL)
		free(chunk->infile);
	if (chunk->delimiter != NULL)
		free(chunk->delimiter);
	if (chunk->outfile != NULL)
		free(chunk->outfile);
	if (chunk->og != NULL)
		free(chunk->og);
	if (chunk->cmd_n_args != NULL)
		free_db_str(chunk->cmd_n_args);
	if (chunk->path != NULL)
		free(chunk->path);
	free(chunk);
}

void	free_exec(t_execlist *execl)
{
	int	i;

	i = -1;
	while (execl->chunk && execl->chunk[++i] != NULL)
		free_chunk(execl->chunk[i]);
	//if (execl->my_envp != NULL)
		//free_db_str(execl->my_envp);
	if (execl->pipe_loc != NULL)
		free(execl->pipe_loc);
	free(execl);
}
