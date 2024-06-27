/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/06/27 20:25:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec_action(t_execlist *execl, int **fd, int i, char ***exec_str) //int **redir
{
	open_all_redirs(execl); //acho que ta a dar
	exec_input(execl, fd, i); //ver se é preciso mudar para **execl tambem
	exec_output(execl, fd, i, exec_str);
	if (execl->chunk[i]->blt == 0)
		execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
	else if (execl->chunk[i]->blt == 1)
	{
		blt_central(execl, i, exec_str[i]); //mode 1
		//dup2(coiso, STDOUT_FILENO);
		//blt_central(execl, i, exec_str[i], mode 2);
		if (execl->valid_cmds == 1)
		{
			//printf("in writing: closing reading [%d]\n", execl->env_pipe[0]);
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
mode 2 - echo, pwd (prints)
mode 2 X - cd, export, unset, env? (alguma destas faz print)
se for comandos de apenas print acho que esta maneira é tranquila
mas se for comandos tipo unset export etc, vou tar eliminar ou criar
variavéis 2 vezes etc
posso adicionar mode 1 e mode 2 ao blt, mode 2 é a repeticao e certos comandos
nao fazem mode 2 e nao repetem

e caso seja builtin nem sequer precisa de entrar no exec_input porque nao
usa isso para nada. só preciso é de ter cuidado com fechar todos os pipes fd
(exceto os related?)
ou seja, nem input nem output?
só preciso é de escrever para aqueles dois files simultaneos e preciso dos
file descriptors?

tenho fds, dup2 e closes

builtins
(so dup2 no exec_output)
dup2 outfile
blt_central
dup2 outpipe
blt central
envp write to pipe

X builtins
(execve para outfile)
dup2 outpipe NO MESMO PROCESS
sair ca p fora e execve normal

para os BLT:
-> trazer os fd cá para fora ou guardados no execl
(inserir aqui os espaços dentro do chunk q eles vao ocupar)
(se calhar so preciso de trazer o outfile cá para fora)
-> fazer o dup2 para o segundo (por convençao, la dentro outfile, ca fora
outpipe)

se calhar para evitar grandes mudanças, os BLT trazem já a default redirection
outpipe, fazem primeiro o exec outpipe, e depois é que fazem novo dup2 para outfile
*/

void	exec_launch(t_execlist *execl, int **fd, int i, char ***exec_str)
{
	int	pid;

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
	char ***tmpenv;

	i = 0;
	ft_printf("Inside exec_loop.\n\n\n");
	//print_db_char(execl->my_envp[0]);
	tmpenv = NULL;
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		execl->env_pipe = malloc(2 * sizeof(int));
		//ret = pipe(execl->env_pipe);
		/*if (ret == -1)
			printf("pipe error\n");
		else
			printf("pipes created: [0] = %d, [1] = %d\n", execl->env_pipe[0], execl->env_pipe[1]);*/
	}
	pid = fork();
	if (pid == 0)
		exec_launch(execl, fd, i, exec_str);
	close_pipes(execl, fd, i, 1, 1);
	//sig_handler(3);
	wait(NULL);
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		//printf("cmds = %d, changes[0] = %d\n", execl->valid_cmds, check_changes(execl->chunk[0]));
		//printf("in read: closing writing [%d]\n", execl->env_pipe[1]);
		close(execl->env_pipe[1]);
		free_db_str(execl->my_envp[0]);
		tmpenv = read_from_pipe(execl->env_pipe[0], execl);
		execl->my_envp = tmpenv;
		//printf("\n\n\nnew envs\n");
		//print_db_char(execl->my_envp[0]);
		close(execl->env_pipe[0]);
		//printf("also here\n");
	}
	//free_db_str(execl->my_envp[0]);
	//execl->my_envp = tmpenv;
	ft_printf("\n\n\nexec_loop finished.\n");
}