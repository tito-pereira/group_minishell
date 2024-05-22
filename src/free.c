/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/05/22 18:11:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	//printf("infile freed\n");
	//chunk->infile = NULL;
	if (chunk->delimiter != NULL)
		free(chunk->delimiter);
	//printf("delimiter freed\n");
	//chunk->delimiter = NULL;
	if (chunk->outfile != NULL)
		free(chunk->outfile);
	//printf("outfile freed\n");
	//chunk->outfile = NULL;
	if (chunk->og != NULL)
		free(chunk->og);
	//printf("og freed\n");
	//chunk->og = NULL;
	if (chunk->cmd_n_args != NULL)
		free_db_str(chunk->cmd_n_args);
	//printf("cmd n args freed\n");
	//chunk->cmd_n_args = NULL;
	if (chunk->path != NULL)
		free(chunk->path);
	//printf("path freed\n");
	//chunk->path = NULL;
	free(chunk);
	chunk = NULL;
}

void	free_exec(t_execlist *execl)
{
	int	i;

	i = -1;
	//printf("probably an error in free\n");
	while (execl->chunk && execl->chunk[++i] != NULL)
		free_chunk(execl->chunk[i]);
	//printf("chunks freed\n");
	if (execl->pipe_loc != NULL)
		free(execl->pipe_loc);
	//printf("pipe loc free\n");
	free(execl);
}
