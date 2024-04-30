/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:45:07 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 14:55:46 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
(5) - identificar inpipes e command path

- coloca a flag de inpipe nos respetivos comandos
- substitui o cmd_n_args pelo respetivo char **
	requirido pela função execve

◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options

prototipo da get_next_line
get_next acrescenta um \n ao fim de cada linha?
porque raio o output tem um \n no fim
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
// ou env[3] = ft_strdup(name);

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
// char	*get_next_line(int fd)
// recebe built in name e retorna o seu PATH
// execve da funcao find c o nome como argumento
// pipe the result and return it

char	*get_cmd_name(char *arg)
{
	char	*name;

	name = NULL;
	ft_printf("Getting valid command name.\n");
	if (ft_strncmp("echo", arg, 256) == 0)
		name = ft_strdup("ft_echo.c");
	else if (ft_strncmp("cd", arg, 256) == 0)
		name = ft_strdup("ft_cd.c");
	else if (ft_strncmp("pwd", arg, 256) == 0)
		name = ft_strdup("ft_pwd.c");
	else if (ft_strncmp("export", arg, 256) == 0)
		name = ft_strdup("ft_export.c");
	else if (ft_strncmp("unset", arg, 256) == 0)
		name = ft_strdup("ft_unset.c");
	else if (ft_strncmp("env", arg, 256) == 0)
		name = ft_strdup("ft_env.c");
	else if (ft_strncmp("exit", arg, 256) == 0)
		name = ft_strdup("ft_exit.c");
	if (name != NULL)
		ft_printf("Valid command returned: '%s'\n", name);
	if (name == NULL)
		ft_printf("Non-existant command\n");
	return (name);
}
// aqui vai ser o nome do executavel do built in
// retirar o ".c" quando ja tiver os executaveis prontos

char	*find_path(char	*arg)
{
	char	*name;
	char	*path;

	ft_printf("Finding our built-in command PATH:\n");
	name = get_cmd_name(arg);
	if (name == NULL)
		return (NULL);
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
// returns 0 in case of non-existant built in command
// returns 1 in case of an existant one

/*
arg[0] = command name
args[1, ...] = command args

first command = check if valid; substitute path
path mt especifico
Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).

cmd_n_args ja vem do ponto 4 onde eu separo todos os argumentos e uso regras
de unclosed commas e verifico isso tudo
vem apenas untreated separated arguments
In point 5, i identify the command executable by parsing the first written word
(or i create an error condition in case of non existant built in command)

eu vou ter os builtins em minishell/builtins
agora, onde eu vou ter o minishell pode variar completamente, bem como o
proprio PWD durante a execucao do minishell [PWD + CD commands]
bem uma coisa e certa, eu tou a construir isto so para linux / Ubuntu devido a
maneira como uso o comando fixo usr/bin/find
comandos de OS tem o comando "which", mas os programas criados por mim ja nao
uso qual? find?

find /path/to/search -name "filename"
find . -name "example.txt"
find . -name "*.txt"

deixar os cmd_n_args tal como vem do ponto 4, apenas verificar se sao validos

*/

int	arg_id(t_execlist *execl)// int *exit_stt)
{
	int	i;

	i = -1;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (5): arg_id;\n");
	while (execl->chunk[++i] != NULL) // iterar os chunks todos pelo chunk_id
	{
		ft_printf("Getting command id from chunk n°%d:\n", i);
		if (chunk_id(execl->chunk[i]) == 0)
		{
			perror("Invalid shell command");
			//free_exec(execl);
			//exit(0);
			execl->exit_stt = 127;
			return(0);
		}
		if (i >= 1)
			execl->chunk[i]->inpipe = 1;
	}
	ft_printf("Finished the arg id parsing.\n");
	return(1);
}

/*
. [INPIPE] so existe inpipe flag a partir do primeiro porque
so pode haver inpipe se houver um comando anterior e o primeiro comando
nao tem nenhum anterior
. [OUTPIPE] todos vão ter outpipe flag, só preciso de proc os inpipes
-> se todos tem outpipes, preciso mesmo de uma flag p isso?
ou em vez de ser uma flag e um indicador de nome?

opção 1
minhas próprias funções implementadas do inicio
executor cria buffers para usar como pipes entre execuções de funções

opção 2
execve
transformar os builtins em programas executaveis e nao funções

- identificar comando, arg_id
- sabendo qual o comando, recolher o seu path
	-> fork, execve(path/find, "builtin");
	-> dup2 output to pipe;
	-> pipe "find" result do cmd_n_arg[0]
- substituir cmd_n_args[0] (nome do comando) pelo seu path
- lidar tambem com as pipe_flags do executor
(ja nao me lembro bem)

int execve(const char *pathname, char *const argv[], char *const envp[]);
execve(char *path, char **args, char **envp)
args[0] == path;
*/
