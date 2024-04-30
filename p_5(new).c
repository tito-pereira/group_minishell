/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5(new).c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:35:51 by tibarbos          #+#    #+#             */
/*   Updated: 2024/04/30 16:21:13 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_execve(char *name)
{
	char	**env;

	ft_printf("Creating char **env.\n");
	env = malloc(5 * sizeof(char *));
	env[0] = ft_strdup("/usr/bin/find");
	env[1] = ft_strdup(".");
	env[2] = ft_strdup("-name");
	env[3] = name;
	env[4] = NULL;
	return (env);
}

char	*get_path(char *name)
{
	char	*path;
	char	**env;
	int		*fd;
	int		pid;

	fd = malloc(2 * sizeof(int));
	ft_printf("Forking another process to find our PATH.\n");
	if (pipe(fd) == -1)
		return (NULL);
	env = find_execve(name);
	ft_printf("char **'env' created successfully:\n");
	ft_printf("[0]:%s; [1]:%s; [2]:%s; [3]:%s; [4]:%s;\n", env[0], env[1], env[2], env[3], env[4]);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close (fd[1]);
		execve(env[0], env, ENV_VAR);
		ft_printf("execve error ocurred.\n");
	}
	wait(0);
	free_db(env);
	close(fd[1]);
	path = get_next_line(fd[0]);
	ft_printf("get_next_line return: '%s'\n", path);
	close(fd[0]);
	return (path);
}

char	*find_path(char	*arg)
{
	char	*name;
	char	*path;

	ft_printf("Finding our built-in command PATH:\n");
	//name = get_cmd_name(arg);
	//if (name == NULL)
		//return (NULL);
	path = get_path(name);
	ft_printf("PATH discovered.\n");
	if (path == NULL)
		return (NULL);
	path = rmv_newline(path);
	ft_printf("PATH found: '%s'\n", path);
	return (path);
}

int	chunk_id(t_chunk *chunk)
{
	char	*path;
	char	*old;

	path = find_path(chunk->cmd_n_args[0]);
	if (path == NULL)
		return (0);
	ft_printf("Swapping old cmd_n_arg[0] with PATH.\n");
	old = chunk->cmd_n_args[0];
	chunk->cmd_n_args[0] = path;
	free (old);
	ft_printf("Swap successfull.\n");
	return (1);
}

//-----------------//

int	check_builtin(char *arg)
{
	ft_printf("Checking if builtin or not.\n");
	if (ft_strncmp("echo", arg, 256) == 0
		|| ft_strncmp("cd", arg, 256) == 0
		|| ft_strncmp("pwd", arg, 256) == 0
		|| ft_strncmp("export", arg, 256) == 0
		|| ft_strncmp("unset", arg, 256) == 0
		|| ft_strncmp("env", arg, 256) == 0
		|| ft_strncmp("exit", arg, 256) == 0)
		return (1);
	return (0);
}

int	arg_id(t_execlist *execl)// int *exit_stt)
{
	int	i;

	i = -1;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (5): arg_id;\n");
	while (execl->chunk[++i] != NULL)
	{
		ft_printf("Getting command id from chunk n°%d:\n", i);
		if (i >= 1)
			execl->chunk[i]->inpipe = 1;
		execl->chunk[i]->blt = check_builtin(execl->chunk[i]->cmd_n_args[0]);
		if (execl->chunk[i]->blt == 1)
		{
			ez;
		}
		else if (chunk_id(execl->chunk[i]) == 0)
		{
			perror("Command path finding error");
			execl->exit_stt = 127; // ???
			return(0);
		}
	ft_printf("Finished the arg id parsing.\n");
	return(1);
}

/*
if (BUILTIN) , flag == 1
arg[0] = path/builtft
arg[1] = (char *)cmd_n_args;

else (terminal) , flag == 0
path_n_args || modify cmd_n_args
arg[0] = path/terminal
arg[1] = arg normal

por default,
execl->chunk[i]->path = NULL;
senao, mudar apenas nos builtins;

vou ter que retirar aquela condicao de erro porque vou aceitar todos os
comandos e so verifico os erros no executor
*/