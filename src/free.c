/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/06/26 19:08:36 by marvin           ###   ########.fr       */
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

int	*free_int_arr(int *str)
{
	free(str);
	return (NULL);
}

/*
typedef struct s_chunk {
	char	**infiles; //nome dos files
	int		nmb_inf; //numero total
	int		*here_dcs; //valores da flag
	int		heredoc; // --- //
	char	*here_file; // --- //
	char	*delimiter;
	char	**outfiles; //
	int		nmb_outf; //
	int		append;
	int		*app_dcs; //
	char	*og;
	char	**cmd_n_args;
	char	*path; // --- //
	int		inpipe;
	int		inpfd; // --- //
	int		outpipe; // --- //
	int		outpfd; // --- //
	int		blt;
}	t_chunk;

sera que preciso de colocar tudo de volta a NULL?
reutilizo a mesma struct em iteraçoes seguintes?
*/

t_chunk	*free_chunk(t_chunk *chunk)
{
	//char **infiles
	if (chunk->infiles)
		chunk->infiles = free_db_str(chunk->infiles);
	//int *here_dcs
	if (chunk->here_dcs)
		chunk->here_dcs = free_int_arr(chunk->here_dcs);
	//char *here_file
	if (chunk->here_file)
		chunk->here_file = free_str(chunk->here_file);
	//char *delimiter
	if (chunk->delimiter)
		chunk->delimiter = free_str(chunk->delimiter);
	//char **outfiles
	if (chunk->outfiles)
		chunk->outfiles = free_db_str(chunk->outfiles);
	//int *app_dcs
	if (chunk->app_dcs)
		chunk->app_dcs = free_int_arr(chunk->app_dcs);
	//char *og
	if (chunk->og)
		chunk->og = free_str(chunk->og);
	//char **cmd_n_args
	if (chunk->cmd_n_args)
		chunk->cmd_n_args = free_db_str(chunk->cmd_n_args);
	//char *path
	if (chunk->path)
		chunk->path = free_str(chunk->path);
	free(chunk);
	return (NULL);
}

/*
typedef struct s_execlist {
	t_chunk	**chunk;
	int		cmd_nmb;
	int		pipe_nmb;
	char	***my_envp;
	int		valid_cmds;
	int		*pipe_loc;
	int		*exit_stt;
	int		*to_end;
	int		*env_pipe;
}	t_execlist;
*/

t_execlist	*free_exec(t_execlist *execl)
{
	int	i;

	i = -1;
	//t_chunk **chunk
	printf("free chunks\n");
	while (execl->chunk && execl->chunk[++i] != NULL)
		execl->chunk[i] = free_chunk(execl->chunk[i]);
	//int *pipe_loc
	printf("free pipe locs\n");
	if (execl->pipe_loc)
		execl->pipe_loc = free_int_arr(execl->pipe_loc);
	//int *exit_stt
	printf("free exit stt\n");
	//if (execl->exit_stt)
		//execl->exit_stt = free_int_arr(execl->exit_stt);
	//int *to_end
	printf("free to end\n");
	//if (execl->to_end)
		//execl->to_end = free_int_arr(execl->to_end);
	//int *env_pipe
	printf("free env pipe\n");
	//if (execl->env_pipe)
		//execl->env_pipe = free_int_arr(execl->env_pipe);
	//char ***my_envp
	i = -1;
	printf("free my envp\n");
	/*while (execl->my_envp && execl->my_envp[++i] != NULL)
		execl->my_envp[i] = free_db_str(execl->my_envp[i]);
	if (execl->my_envp)
	{
		free(execl->my_envp);
		execl->my_envp = NULL;
	}*/
	printf("free execl\n");
	free(execl);
	printf("done frees\n");
	return (NULL);
}

/*
exit stt
to end
env pipe
my envp??

nao foram inicializados provavelmente

also , redirections
*/