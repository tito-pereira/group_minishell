/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:38 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 14:57:42 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_chunk(t_execlist *execl)
{
	int	*fd;
	int	pid;
	int	c;

	c = execl.current;
	fd = malloc (2 * sizeof(int));
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (inpipe == 1)
			dup2(execl.chunk[c].inpfd, STDIN_FILENO);
		else if (inpipe == 0 && infile != NULL)
			dup2(open(execl.chunk[c].infile, O_RDONLY), STDIN_FILENO);
		if (outfile != NULL)
			dup2(open(execl.chunk[c].outfile, O_RDONLY), STDOUT_FILENO);
		if (outpipe != NULL)
			execl.chunk[c + 1].inpfd = execl.chunk[c].outpfd; //a ser usado no comando seguinte
		execve(execl.chunk[c].cmd_n_args[0], execl.chunk[c].cmd_n_args, ENV_VAR);
	}
	wait(0);
	close(fd[1]);
	return (fd[0]);
}

/*
pega no fd do ultimo pipe fd e copia tudo (getnextline) para o STDOUT
*/
void	last_output(int last)
{
	char	*ret;
	int		sig;

	sig = 1;
	while (sig == 1)
	{
		ret = get_next_line(last);
		if (ret != NULL)
		{
			ft_printf("%s\n", ret);
			free(ret);
		}
		else
			sig = 0;
	}
	close(last);
}
/*
retornar o errno apenas no fim, nos passos intermédios
nao é realmente necessário

o ultimo comando tem que executar para o terminal
por isso tenho que ligar o pipe final e copiar o output todo
para o STDOUT
tou a pensar usar get_next_line(pipe->STDOUT)
*/

void	execlist_exe(t_execlist *execl)
{
	int	i;
	int	last;

	if (execl.cmd_nmb != (execl.pipe_nmb + 1))
	{
		perror("Number of pipes and commands is not compatible");
		free_exec(execl);
		exit (0);
	}
	i = -1;
	while (execl.chunk[++i] != NULL)
	{
		last = exec_chunk (execl);
		execl.current++;
	}
	execl.erno = errno; //errno do ultimo comando antes de ser contaminado c a getnextline
	last_output(last);
	//return (1);
}

/*
execve()

char *PATH, char **ARGS, char **execl->my_envp

av[1] = (char *)"./builtins/builtfunct";
av[2] = (char *)execl; (atualizar o error status, se for, p 126)
av[3] = (char *)execl->chunk[i]->cmd_n_args (cmd_n_args ponto 4)

execl->my_envp; //step 1
execl->path; //step 5
cmd_n_args mantém-se do ponto 4

Exit status 2: Misuse of shell builtins (e.g., incorrect usage of a command).
(built in failed)
Exit status 126: Permission problem or command is not executable.
(executor failed)

.mudar o executavel do step 5 para sempre o mesmo;
.deixar um united char **args para usar aqui;
.mudar as t_mini para um int *error
.fazer mesmo o executor
*/

/*
av[1] = int option;
av[2] = t_mini *ms;
av[3] = char **cmd;
av[4] = char **envp;

1	ft_cd(t_mini *ms, char **cmd, char **env);
2	ft_echo(t_mini *ms, char **cmd);
3	ft_env(t_mini *ms, char **cmd, char **envp);
4	ft_exit(t_mini *ms, char **cmd);
5	ft_export(t_mini *ms, char **cmd, char **envp);
6   ft_pwd(t_mini *ms, char **cmd, char **envp);
7	ft_unset(t_mini *ms, char **cmd, char **envp);
*/