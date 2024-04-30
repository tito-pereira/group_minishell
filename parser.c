/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:45:15 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 15:26:05 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
retirar todos os free_execl individuais e fazer um geral na main
(c exit status != 0)
(1) - V
(2) - V
(3) - V
(4) - V
(5) - V
exit_stt:
(1) - V, 1
(2) - V, 1
(3) - V, 1
(4) - V, 1
(5) - V, 127
(exe) - por fazer (126)
(blt) - por fazer (2)

talvez tenha q retornar o error_stt a main e nao na struct execl

change *execl -> &execl (????) q raio é isto
(1) - por fazer
(2) - por fazer
(3) - por fazer
(4) - por fazer
(5) - por fazer

error/individual exit(0) list:
(1) (DONE)
. pipe_chunks (x2)
. chunk_create
(2)
. check_redir (x2);
(pode haver mais na 2a)
(3)
. handle_env_var (x2);
(pode haver mais)
(4)
. ret from cmd_separator (x2);
. (pode haver mais na 4a)
(5) (DONE)
. chunk_id returns 0;

Exit status 1: Generic error code indicating unspecified error.
(1, 2, 3, 4)
Exit status 2: Misuse of shell builtins (e.g., incorrect usage of a command).
(built in failed)
Exit status 126: Permission problem or command is not executable.
(executor failed)
Exit status 127: Command not found or executable cannot be invoked.
(5, arg_id)
Exit status 128+n: Fatal error signal n (where n is a signal number).
Exit status 130: Command terminated by Ctrl+C (SIGINT).
(signal ctrl-D)
Exit status 255: Exit status out of range or undefined.
*/

/*
$?:
- return status of previously parsed/executed command. By default, at the
beginning, is 0; in case of a succesfull command is also 0, but in case
of error becomes a non-zero integer corresponding to the exit status type;
*/

ENVIRONMENT VARIABLES:

a questão do handle environment variables acho que é mais fácil do que
parece. basta fazer um pequeno if $, o que quer que seja que vem a seguir nós
usamos com as funções ttcenas. se for válido vai retornar algo, se for válido vai
retornar nulo ou erro e nós simplesmente retornamos nulo e dizemos inválido. ez.
escuso de escrever todos os cenários possíveis.
$<opção>
$? (o último comando $ a ser pesquisado, ir buscar à história
em caso de não haver nenhum, testar e ver como é suposto fazer)

| - pipes
<> - redirections
"" - interpretar aspas como expressões individuais, usa $
'' - interpretar aspas como expressões individuais, nao usa $
$ - caracteres especiais

5. unclosed quote errors (apenas numeros pares e ordem logica)
	"" podem conter '' dentro?
	'' podem conter "" dentro?
1. pipe counter (cuidado com || symbol)
2. arg manager (split condicionada)
3. redirections e in/out files
4. special characters ($)
5. pipex ou direct execution

---
- fiz uma lista de todos os requerimentos de regras, erros e output do parser
- tentei criar um esquema geral e prioridades de codigo
	(bem como alguns principios de boas praticas, tentar modular ao maximo
	para eventuais novas regras, tentar criar um esquema top/down logo do
	inicio bem struturado em vez de constante hard code que se torna
	impossivel de ler, etc)
- tentar criar um proto codigo que posso ir modificando ao longo de tempo
- eventualmente ter um produto final

a adicionar (bónus):
- () e subshells
- operações || e && com chunks não divididas
- wildcards * e como procurar todos os files ou directories por nomes
(nao posso simplesmente passar o wildcard como arg para a função a realizar exec?)

/*
verificar se o numero de pipes e comandos bate bem
()
;
podem ser construidos "acima" deste modelo sem problema nenhum,
é bastante modulavel
*/
	
- "" podem conter '' dentro? (sim)
- '' podem conter "" dentro? (sim)
- um pipe precisa de estar " | " ou pode ter comandos colados? (pode) 
- e as redirections tambem? (tambem podem ter coisas coladas)
- e as aspas tambem? (same)
(aspas e argumentos e nomes de executaveis eu sei que precisam de estar corretamente
espaçados para se distinguirem c exceção de aspas)

basicamente, deu me a ideia de tratar primeiro de um parser que separa em char ntroduzir erros de '\0':
-1 erro de unclosed quotes
0 grupo errado
*/*
a volta de cada | symbol, que percorre um a um à procura de redirections e depois
scaneia o texto seguinte (skip space e tab, texto, acaba c novo whitespace) e
depois construir a partir daí, com substr manuais

MANDATORY PARSER:

//cada chunk, aka, comando singular entre pipes
typedef struct s_chunk {
	char	*infile; //nome se custom, NULL se stdin
	int		heredoc; //1 se sim, 0 se nao
	char	*delimiter; //com nome ou NULL
	char	*outfile; //nome se custom, NULL se stdout
	int		append; //0 se não (truncate), 1 se sim (append)
	char	*og; //user input
	char	**cmd_n_args; //final parsed command & arguments
	int		inpipe; //1 se for precedido de um pipe
	int		inpfd; //se sim, o seu fd
	int		outpipe; //1 se for redirecionado para um pipe
	int		outpfd; //se sim, o seu fd
}	t_chunk;

//conjunto de todos os chunks e todos os pipes
typedef struct s_execlist {
	t_chunk	**exe_order;
	int			cmd_nmb;
	int			pipe_nmb;
}	t_execlist;

(1) - separar em pipe_chunks
(2) - ler <> (condições de erro para redirections mal aplicadas)
(3) - tratar das special characters
(4) - separar em "", '' chunks (contem condições de erro para unclosed)
(5) - identificar in/out pipes e command path

/*
preencher inpipe, outpipe, pipecmds, cmdnbr,
e fazer a pesquisa e preencher cmd_n_agrs para o execve

1 (Y)
2 (Y)
3 (X)
4 (Y)
5 (Y)

---
(0)
*/

/*

---
(1)
//separar em chunks divididos por pipes que significam comandos diferentes

int	pipe_counter(char *input)
{
	int	c;
	int	i;

	c = 0;
	i = -1;
	while (input[++i] != '\0')
	{
		if (input[i] == '|')
		{
			if (input[i + 1] == '|')
				i++;
			if (input[i + 1] != '|')
				c++;
		}
		//if (input[i] == '|' && input[i + 1] != '|')
			//c++;
	}
	return (c);
}

char	**pipe_divider(int count)
{
	char	**og_group;
	int		total;

	total = count + 1
	og_group = malloc (total * sizeof(char *));
	return (og_group);
}

char	**pipe_chunks(char *input)
{
	int			c;
	t_execlist	*exel;

	c = pipe_counter(input);
	exel = malloc (sizeof(t_execlist));
	exel.cmd_nmb = c + 1;
	exel.pipe_nmb = c;
	exel.exe_order = malloc ((c + 1) * sizeof(t_exehelp *));
	//using pipe chunks e pipe_divider
}

---
(2)
//ler e anotar redirections e file names em cada chunk

char	*get_name(char *str, int i)
{
	char	*name;

	ez;
	return (name);
}

void	redir_checker(t_execlist *chunk)
{
	int	i;

	i = 0;
	while(chunk.og[i] != '\0')
	{
		if(chunk.og[i] == '<')
		{
			if(chunk.og[i] == '<')
			{
				ez;
				infile = get_name(i);
			}
			else if(chunk.og[i] != '<')
			{
				ez;
				infile = get_name(i - 1);
			}
		}
		else if(chunk.og[i] == '>')
		{
			if(chunk.og[i] == '>')
			{
				chunk.append = 1;
				outfile = get_name(i);
			}
			if(chunk.og[i] != '>')
			{
				ez;
				outfile = get_name(i - 1);
			}
		}
	}
}

*/

/*
---
(3)

substituir special characters $
talvez usar as funções tt para is buscar as variáveis

nao esquecer que "" suporta $ mas '' já não


char	*strjoin / strcat?

char	*get_spec_char(int *a, int *b, char *chunk)
{
	int		i;
	//char	*piece;

	i = -1;
	while (chunk[++i] != '\0')
	{
		if (chunk[i] = '$')
		{
			*a = i;
			while (chunk[i] != 9 && chunk[i] != 32)
				i++;
			if (chunk[i] == 9 && chunk[i] == 32)
				*b = i;
		}
	}
	//piece = ft_substr(chunk, a, b);
	return (ft_substr(chunk, a, b));
}


void	special_char(char *chunk)
{
	int		a; //posicao inicial do $
	int		b; //posicao final do $
	char	*spec;
	char	*first;
	char	*secnd;

	a = 0;
	b = 0;
	spec = get_spec_char(&a, &b, chunk);
	if (spec != NULL)
	{
		substr1 (0, a);
		substr2 (b, end);
		strcat1 (substr1, spec);
		strcat2 (strcat1, substr2);
	}
	if (*a > 0)
		first = (0, a, chunk);
	if (*b < ft_strlen(chunk))
		secnd = ft_substr(b, ft_strlen(chunk), chunk);
	ft_strcat(first, spec, secnd);
	//else, retorna normalmente sem fazer nada
}

---
(4)
//separar em cmd_n_arg incluindo "" e '', ft_substr c posições

- spaces / ! spaces
- "" (onde '' contam como literals)
- '' (onde "" contam como literals)

execlist.cmd_n_args = arg_separator(execlist.og);


int	arg_counter(char *chunk)
{
	int	c;
	int	i;
	int	j;

	i = 0;
	j = 0;
	c = 0;
	while (chunk[i] == 9 || chunk[i] == 32)
		i++;
	if (chunk[i] == "")
	{
		while ()
			j++;
		if ()
			c++;
	}
	if (chunk[i] == '')
	return (c);
}

char	**arg_separator(char *chunk)
{
	int		i;
	char	**cmd_n_arg;

	i = -1;
	if (arg_counter(chunk) == -1)
		return (NULL); //condição de erro
	cmd_n_arg = malloc (arg_counter(chunk) * sizeof(char *));
	while (chunk[++i] != '\0')
	{
		if (chunk[i] == 9 || chunk[i] == 32)
		{
			ez;
		}
		else if (chunk[i] == "")
		{
			while (chunk[i + j] != "" && chunk[i + j] != '\0')
			{
				j++;
				if (chunk[i + j] == )
			}
		}
	}
}

---
(5) - identificar inpipes e command path


◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options


char	*get_path(char *tmp)
{
	char	*path;
	char	**env;
	int		*fd;
	int		pid;

	if (pipe(fd) == -1)
		return (NULL);
	env = find_execve(tmp);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		execve("/usr/bin/find", env, ENV_VAR);
	}
	wait (0);
	close (fd[1]);
	path = get_next_line(fd[0]);
	close (fd[0]);
	return (path);
}

char	*get_tmp(char *arg)
{
	char	*tmp;
	tmp = NULL;
	if (ft_strncmp("echo", arg, 256) == 0)
		tmp = ft_strcpy("echo");
	else if (ft_strncmp("cd", arg, 256) == 0)
		tmp = ft_strcpy("cd");
	else if (ft_strncmp("pwd", arg, 256) == 0)
		tmp = ft_strcpy("pwd");
	else if (ft_strncmp("export", arg, 256) == 0)
		tmp = ft_strcpy("export");
	else if (ft_strncmp("unset", arg, 256) == 0)
		tmp = ft_strcpy("unset");
	else if (ft_strncmp("env", arg, 256) == 0)
		tmp = ft_strcpy("env");
	else if (ft_strncmp("exit", arg, 256) == 0)
		tmp = ft_strcpy("exit");
	return (tmp);
}

char	*find_path(char	*arg)
{
	int		ret;
	char	*tmp;
	char	*path;

	tmp = get_tmp(arg);
	if (tmp == NULL)
		return (NULL);
	path = get_path(tmp);
	if (path == NULL)
		return (NULL);
	return (path);
}


void	chunk_id(t_chunk *chunk)
{
	char	*path;
	char	*kill;

	path = find_path(chunk.cmd_n_args[0]);
	if (path == NULL)
		return (0);
	//substituir arg[0] original pelo path e retornar
	kill = chunk.cmd_n_args[0];
	chunk.cmd_n_args[0] = path;
	free (kill);
	return (1);
}


arg[0] = command name
args[1, ...] = command args

first command = check if valid; substitute path
path mt especifico
Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).
comandos de OS tem o comando "which"
programas criados por mim? find?

find /path/to/search -name "filename"
find . -name "example.txt"
find . -name "*.txt"


void	arg_id(t_execlist *execl)
{
	int	i;

	i = -1;
	//command paths
	while (execl.chunk[++i] != NULL)
	{
		if (chunk_id(execl.chunk[i]) == 0);
		{
			perror("Invalid command '%s'", execl.chunk[i].cmd_n_args[0]);
			free_exec(execl);
			exit(0);
		}
		//inpipe flag
		if (i >= 1)
			execl.chunk[i].inpipe = 1;
	}
}



---.---.---.---.---.---.---.---.---
			BONUS	|	|	|	|
---.---.---.---.---.---.---.---.---
/*
subshell - vários (execl_group)
execl_group - execlist 1 + if (|| &&) + execlist 2
execlist - vários exe_chunks (pipes)

- () e subshells
- operações || e && com chunks não divididas
- wildcards * e como procurar todos os files ou directories por nomes
(nao posso simplesmente passar o wildcard como arg para a função a realizar exec?)
*/
---
// () e subshells

/*
typedef struct s_subshell {
	ez;
}	t_subshell;

non-()-parts and ()-parts
substituir ()-parts com o seu output
depois de retirados todos os parentesis, executar normalmente
como um execl_group


---
// &&, ||, execl_groups

typedef struct s_oper_exlg {
	int			operator; //0 se &&, 1 se ||;
	int			errno;
	t_execlist	*execl;
}	t_oper_exlg;

typedef struct s_execl_group {
	t_execlist	*first;
	t_oper_exlg	*operator;
}	t_execl_group;
*/