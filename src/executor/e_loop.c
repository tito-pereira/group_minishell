/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/07/01 02:33:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
pwd, echo, export deixaram de dar qq tipo de output no terminal
*/

void	blt_action(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	int	n_file;
	int	tmp;

	n_file = execl->chunk[i]->nmb_outf;
	tmp = 0;
	//printf("inside BLT action\n");
	close_pipes(execl, fd, i, 0, 1); //close non related
	if (execl->chunk[i]->outfiles)
	{
		//printf("if 1\n");
		if (execl->chunk[i]->app_dcs[n_file] == 1) //append
			tmp = open(execl->chunk[i]->outfiles[n_file], \
			O_RDWR | O_CREAT | O_APPEND, 0644);
		else if (execl->chunk[i]->app_dcs[n_file] == 0) // truncate
			tmp = open(execl->chunk[i]->outfiles[n_file], \
			O_RDWR | O_CREAT | O_TRUNC, 0644);
		dup2(tmp, STDOUT_FILENO);
		close(tmp); //outfile
		blt_central(execl, i, exec_str[i]);
		close(tmp);
	}
	else if (!execl->chunk[i]->outfiles && (i + 1) < execl->valid_cmds) //outpipe
	{
		//printf("else if 2\n");
		dup2(fd[i + 1][1], STDOUT_FILENO);
		close(fd[i + 1][1]); //outpipe
		blt_central(execl, i, exec_str[i]);
	}
	else
	{
		//printf("else if 3\n");
		blt_central(execl, i, exec_str[i]); //terminal
	}
	//printf("getting out\n");
	close_pipes(execl, fd, i, 1, 0); //close related
}

/*
X input
outfile
	- append
	- trunc
X outfile
	- outpipe
	- terminal
*/

void	exec_action(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	//printf("begin exec action %d\n", i);
	//support_print(execl, i);
	if (execl->chunk[i]->blt == 0)
	{
		exec_input(execl, fd, i);
		exec_output(execl, fd, i);
		//printf("end exec action %d\n", i);
		execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
	}
	else if (execl->chunk[i]->blt == 1)
	{
		//printf("BLT command %d\n", i);
		blt_action(execl, fd, i, exec_str);
		if (execl->valid_cmds == 1)
		{
			//printf("inside BLT env pipe\n");
			close(execl->env_pipe[0]);
			//write(execl->env_pipe[1], execl->my_envp, sizeof(char ***));
			write_to_pipe(execl->env_pipe[1], execl->my_envp);
			//printf("in writing: closing writing [%d]\n", execl->env_pipe[1]);
			close(execl->env_pipe[1]);
		}
	}
	exit(0);
}

/*
mode 2 é a segunda iteração/repeticao das BLT e so alguns comandos resultam

e caso seja builtin nem sequer precisa de entrar no exec_input porque nao
usa isso para nada. só preciso é de ter cuidado com fechar todos os pipes fd
(exceto os related?)
ou seja, nem input nem output?
só preciso é de escrever para aqueles dois files simultaneos e preciso dos
file descriptors?
tenho fds, dup2 e closes, acho que nao falta nada

portanto, preciso dos fds
- arranjar fd[] de outfile e outpipe
outfile é outfiles[app_nmb] ou algo assim, nmb_outf acho
execl->chunk[i]->outfiles[n_file] ??? isto é filename
creio que outpipe seja fd[i + 1] né?
*/

void	exec_launch(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	int	pid;

	//printf("in exec launch. exit stt is %d\n", (*execl->exit_stt));
	if ((i + 1) < execl->valid_cmds)
	{
		i++;
		//ft_printf("launching other [%d]\n", (i - 1));
		pid = fork();
		if (pid == 0)
		{
			exec_launch(execl, fd, i, exec_str);
			//ft_printf("out of launch [%d]\n", i);
			//exit(0);
		}
		i--;
	}
	pid = fork();
	if (pid == 0)
		exec_action(execl, fd, i, exec_str); //redir
	else
	{
		close_pipes(execl, fd, i, 1, 1);
		//ft_printf("lets wait for first one[%d]\n", i);
		pid = wait(NULL);
		if ((i + 1) < execl->valid_cmds)
		{
			//ft_printf("lets wait for scnd one[%d]\n", i);
			wait(NULL);
		}
		//ft_printf("launch done and waited [%d]\n", i);
	}
	//ft_printf("fora action env = %s\n", execl->my_envp[0][52]);
	exit(0);
	//ft_printf("exec launch out [%d]\n", i);
}

int	check_changes(t_chunk *chunk)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(chunk->cmd_n_args[0], "cd", 3) == 0)
		ret = 1;
	if (ft_strncmp(chunk->cmd_n_args[0], "export", 7) == 0)
		ret = 1;
	if (ft_strncmp(chunk->cmd_n_args[0], "unset", 6) == 0)
		ret = 1;
	return (ret);
}

void	exec_loop(t_execlist *execl, int **fd, char ***exec_str)
{
	int	i;
	int	pid;
	//int	ret;
	//char ***tmpenv;

	i = 0;
	//printf("in exec_loop. exit stt is %d\n", (*execl->exit_stt));
	//ft_printf("Inside exec_loop.\n\n\n");
	//print_db_char(execl->my_envp[0]);
	//ret = 0;
	//tmpenv = NULL;
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		//printf("creating the envp pipes:\n");
		execl->env_pipe = (int *)ft_calloc(2, sizeof(int));
		pipe(execl->env_pipe);
		//if (ret == -1)
			//printf("pipe error\n");
		//else
			//printf("pipes created: [0] = %d, [1] = %d\n", execl->env_pipe[0], execl->env_pipe[1]);
	}
	//printf("after env pipe exec_loop. exit stt is %d\n", (*execl->exit_stt));
	open_all_redirs(execl);
	//printf("after open redirs exec_loop. exit stt is %d\n", (*execl->exit_stt));
	ft_printf("Inside exec_loop.\n\n\n");
	pid = fork();
	if (pid == 0)
		exec_launch(execl, fd, i, exec_str);
	close_pipes(execl, fd, i, 1, 1);
	wait(NULL);
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		//printf("cmds = %d, changes[0] = %d\n", execl->valid_cmds, check_changes(execl->chunk[0]));
		//printf("in read: closing writing [%d]\n", execl->env_pipe[1]);
		close(execl->env_pipe[1]);
		free_db_str(execl->my_envp[0]);
		//tmpenv = read_from_pipe(execl->env_pipe[0], execl);
		//execl->my_envp = tmpenv;
		execl->my_envp = read_from_pipe(execl->env_pipe[0], execl);
		/*if (execl->my_envp)
		{
			printf("\nnew envs\n");
			print_db_char(execl->my_envp[0]);
		}
		else
			printf("there are no new envs\n");*/
		close(execl->env_pipe[0]);
		//printf("also here\n");
	}
	//free_db_str(execl->my_envp[0]);
	//execl->my_envp = tmpenv;
	ft_printf("\n\n\nexec_loop finished.\n");
	printf("out exec_loop. exit stt is %d\n", (*execl->exit_stt));
}