/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:48 by marvin            #+#    #+#             */
/*   Updated: 2024/06/30 03:26:43 by marvin           ###   ########.fr       */
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

t_execlist	*free_exec(t_execlist *execl, int mode)
{
	int	i;

	i = -1;
	printf("free mode %d\n", mode);
	//t_chunk **chunk
	//printf("free chunks\n");
	while (execl->chunk && execl->chunk[++i] != NULL)
		execl->chunk[i] = free_chunk(execl->chunk[i]);
	//char ***my_envp
	if (execl->my_envp && mode == 2)
	{
		*(execl->my_envp) = free_db_str(*(execl->my_envp));
		free(*(execl->my_envp));
		execl->my_envp = NULL;
	}
	//int *pipe_loc
	//printf("free pipe locs\n");
	if (execl->pipe_loc)
		execl->pipe_loc = free_int_arr(execl->pipe_loc);
	//int *exit_stt
	//printf("free exit stt\n");
	//if (execl->exit_stt)
		//execl->exit_stt = free_int_arr(execl->exit_stt);
	//int *env_pipe
	//printf("free env pipe\n");
	if (execl->env_pipe)
		execl->env_pipe = free_int_arr(execl->env_pipe);
	//printf("free execl\n");
	free(execl);
	//printf("done frees\n");
	return (NULL);
}

/*
ERROR
export + exit
(provavelmente as my_envp)
tenho mesmo que ir ver ao executor como fiz honestamente

- ja retirei o to_end do header e n me deu nenhum erro.. espero n haver esqueletos prai
- para testar o free my_envp, tem que ser modo 2 (exit ou erros)

ENVP:
envp leva malloc de char ** e varios char *
o envp em si (char ***) nao leva malloc nem deve levar free
c jeito o free_db podia levar char *** e dar free ao char ** original
e returnar NULL
env = create_env
while
{
execl.env = env
execl = new_env (pos exec)
//env = execl.env
}

EXIT_STT
X malloc na main (sem maloc, um int local)
malloc geral da struct, não do pointer especifico
vamos simplesmente nao dar free a exit_stt nenhum e no
fim ver se sobram leaks ou nao

ENV PIPE
p_1 = NULL
exec usado ou nao
free aqui
p_1, etc

exit stt
to end (posso apenas tirar)
env pipe
. o que é?
my envp??
. o que é? a minha duplicacao das env vars
(main.c, p_1.c, etc)
testar com ls que nao altera envp
talvez testar aqueles comandos unset e o crl p verificar que funciona?

FREE (free_execl)
- (provavelmente tenho que esperar pelo error_stt)
- pelo menos resolver os non error stt
*/