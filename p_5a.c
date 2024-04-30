/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:40:54 by tibarbos          #+#    #+#             */
/*   Updated: 2024/04/30 16:45:33 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rmv_newline(char *old)
{
	char				*new;
	unsigned int		len;

	len = ft_strlen(old);
	ft_printf("Checking for newline.\nOld path size: %d;\n", len);
	if (old[len - 1] == '\n')
	{
		ft_printf("Newline detected, preparing to remove.\n");
		new = malloc(len * sizeof(char));
		ft_strlcpy(new, old, len);
		ft_printf("New non-newline path: '%s'\n", new);
		free(old);
		return(new);
	}
	return(old);
}
// size_t	ft_strlcpy(char *s1, const char *s2, size_t size)
// size ja inclui o '\0'
// strlen[size] == '\0', [size - 1] == '\0'

/*
prototipo da get_next_line
get_next acrescenta um \n ao fim de cada linha?
porque raio o output tem um \n no fim

esta funcao aqui remove o \n no fim de cada get_next_line
return
*/

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
	char	*path;

	ft_printf("Finding our built-in command PATH:\n");
	path = get_path(arg);
	ft_printf("PATH discovered.\n");
	if (path == NULL)
		return (NULL);
	path = rmv_newline(path);
	ft_printf("PATH found: '%s'\n", path);
	return (path);
}

int	chunk_id(t_chunk *chunk, char *prog, int opt)
{
	char	*path;
	char	*old;

	if (opt == 1)
		path = find_path(prog); //builtin
	else if (opt == 2)
		path = find_path(chunk->cmd_n_args[0]); //terminal
	if (path == NULL)
		return (0);
	if (opt == 1)
		chunk->path = path; //builtin
	else if (opt == 2)
	{
		ft_printf("Swapping old cmd_n_arg[0] with PATH.\n");
		old = chunk->cmd_n_args[0]; //terminal
		chunk->cmd_n_args[0] = path;
		free (old);
		ft_printf("Swap successfull.\n");
	}
	return (1);
}