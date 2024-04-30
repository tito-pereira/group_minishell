/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:43:38 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 13:18:16 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PARSER STRUCTS:

typedef struct s_chunk {
	char	*infile;
	int		heredoc;
	char	*delimiter;
	char	*outfile;
	int		append; //0 se não (truncate), 1 se sim (append)
	char	*og;
	char	**cmd_n_args;
	int		inpipe; //1 se for precedido de um pipe
	int		inpfd; //se sim, o seu fd, default -1
	int		outpipe; //1 se for redirecionado para um pipe
	int		outpfd; //se sim, o seu fd, default -1
}	t_chunk;

typedef struct s_execlist {
	t_chunk		**chunk;
	int			cmd_nmb;
	int			pipe_nmb;
	int			current; //numero de comando, default 1
	int			erno;
}	t_execlist;

//--- MANDATORY EXECUTOR: ---//

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
		dup2(fd[1], STDOUT);
		if (inpipe == 1)
			dup2(execl.chunk[c].inpfd, STDIN);
		else if (inpipe == 0 && infile != NULL)
			dup2(open(execl.chunk[c].infile, O_RDONLY), STDIN);
		if (outfile != NULL)
			dup2(open(execl.chunk[c].outfile, O_RDONLY), STDOUT);
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
tenho que criar um conversor para a t_mini struct
para conseguir passar os argumentos direitos aqui no executor

vou receber o output 
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

traduzir t_mini *;
deixar um united char **args para usar aqui;
*/

/*
na parte mandatória vou executar todos os outputs finais para o
STDOUT do terminal
Em caso de bónus vou implementar o conceito de subshell usado pelo bash:
- abrir um child process
- executar minishell também no child process
- executar o exec_chunk da parte mandatória no STDOUT desse processo
- retornar o output desse terminal para substituir a expressão em
	parentesis no parent process
*/