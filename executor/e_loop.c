/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:38:06 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/21 13:25:35 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

////////////////////////////////////////////
void	print_db(char **str)
{
	int i = -1;
	while (str[++i] != NULL)
		ft_printf("%s\n", str[i]);
}

void	exec_action(t_execlist *execl, int **fd, \
	int i, char ***exec_str) //int **redir
{
	exec_input(execl, fd, i); //int **redir
	exec_output(execl, fd, i, exec_str);
	if (execl->chunk[i]->blt == 0)
	{
		sig_handler(2);
		execve(exec_str[i][0], exec_str[i], *(execl->my_envp));
	}
	else if (execl->chunk[i]->blt == 1)
	{
		blt_central(execl, i, exec_str[i], execl->err_stt);
		//ft_printf("\n\n\ndentro export env\n");
		//print_db_char(execl->my_envp[0]);
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

void	exec_launch(t_execlist *execl, int **fd, int **redir, \
	int i, char ***exec_str)
{
	int	pid;

	if ((i + 1) < execl->valid_cmds)
	{
		i++;
		//ft_printf("launching other [%d]\n", (i - 1));
		pid = fork();
		if (pid == 0)
		{
			exec_launch(execl, fd, redir, i, exec_str);
			//ft_printf("out of launch [%d]\n", i);
			//exit(0);
		}
		i--;
	}
	pid = fork();
	if (pid == 0)
	{
		exec_action(execl, fd, i, exec_str); //redir
	}
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

void	exec_loop(t_execlist *execl, int **fd, int **redir, char ***exec_str)
{
	int	i;
	int	pid;
	int	ret;
	char ***tmpenv;

	i = 0;
	ft_printf("Inside exec_loop.\n\n\n");
	//print_db_char(execl->my_envp[0]);
	tmpenv = NULL;
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		execl->env_pipe = malloc(2 * sizeof(int));
		ret = pipe(execl->env_pipe);
		if (ret == -1)
			printf("pipe error\n");
		else
			printf("pipes created: [0] = %d, [1] = %d\n", execl->env_pipe[0], execl->env_pipe[1]);
	}
	pid = fork();
	if (pid == 0)
		exec_launch(execl, fd, redir, i, exec_str);
	close_pipes(execl, fd, i, 1, 1);
	sig_handler(3);
	wait(NULL);
	if (execl->valid_cmds == 1 && check_changes(execl->chunk[0]) == 1)
	{
		//printf("\n\n\nim here\n");
		//print_db_char(execl->my_envp[0]);
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
	//ft_printf("\n\n\nin main process envs\n");
	//print_db_char(execl->my_envp[0]);
	ft_printf("\n\n\nexec_loop finished.\n");
}