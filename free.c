/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/05/11 22:43:16 by marvin           ###   ########.fr       */
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

////////////////////////////////////

/*
typedef struct s_chunk {
	char	*infile; // (2) redir_checker
	int		heredoc; // (2) redir_checker
	char	*delimiter; // (2) redir_checker
	char	*outfile; // (2) redir_checker
	int		append; // (2) redir_checker
	char	*og; // (1) parse_execl
	char	**cmd_n_args; // (4) arg_separator
	char	*path; // (5) arg_id
	int		inpipe; // (5) arg_id
	int		inpfd; // executor
	int		outpipe; //acho que já não é necessário
	int		outpfd; // executor
	int		blt; // (5) arg_id
}	t_chunk;

typedef struct s_execlist {
	t_chunk	**chunk;
	int		cmd_nmb;
	int		pipe_nmb;
	char	**my_envp;
	int		valid_cmds;
	int		*pipe_loc;
}	t_execlist;
*/

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
	if (execl->my_envp != NULL)
		free_db_str(execl->my_envp);
	if (execl->pipe_loc != NULL)
		free(execl->pipe_loc);
	free(execl);
}
