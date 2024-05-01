/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:04 by marvin            #+#    #+#             */
/*   Updated: 2024/05/01 12:42:22 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	global_sig;
// 0 neutro, à espera de sinais;
// 1 para redisplay;
// 2 para close;

char	*ft_read()
{
	char	*input;

	while (1)
	{
		ft_printf("global_signal check: %d\n", global_sig);
		input = readline(PROMPT);
		if (input != NULL)
		{
			add_history(input);
			return (input);
		}
		else if (input == NULL && global_sig == 2)
			continue ;
		else if (input == NULL && global_sig != 2)
			exit(0);
	}
}

char	**create_envp(void)
{
	char	**my_envp;
	int		i;

	i = -1;
	while (__environ[++i] != NULL)
		i++;
	my_envp = malloc((i + 1) * sizeof(char *));
	i = -1;
	while (__environ[++i] != NULL)
		my_envp[i] = ft_strdup(__environ[i]);
	my_envp[i] = NULL;
	return(my_envp);
}

/*
eu reconheco SIGINT, mas a readline nao
eu nao reconheco EOF/SIGPIPE, mas a readline sim

nao e que a readline nao reconheca SIGINT, eu e q ja alterei o seu funcionamento
o problema e mesmo o SIGPIPE q nao funciona
*/

void	print_db_char(char **str) {
	int	w = 0;
	while (str[w] != NULL)
		w++;
	ft_printf("cmd_n_args has %d elements;\n", w);
	for (int i = 0; str[i] != NULL; i++) {
		ft_printf("str[%d]: %s;\n", i, str[i]);
	}
}

void	print_exec(t_execlist *execl)
{
	ft_printf("-.-.-.-.-.-.-.-.-.-\n");
	ft_printf(".	PRINT CHUNKS	.\n");
	ft_printf("-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("General overview:\nNumber of commands: %d;", execl->cmd_nmb);
	ft_printf("\nNumber of pipes: %d;\n", execl->pipe_nmb);
	ft_printf("-.-.-.-.-.-.-.-.-.-.\n-.-.-.-.-.-.-.-.-.-.\n");
	for (int i = 0; execl->chunk[i] != NULL; i++) {
		ft_printf("Chunk nª%d\n", i);
		print_db_char(execl->chunk[i]->cmd_n_args);
		//ft_printf("Other info:\nInpipe: %d;", execl->chunk[i]->inpipe);
		ft_printf("Heredoc: %d;\nAppend: %d;\n", execl->chunk[i]->heredoc, execl->chunk[i]->append);
		ft_printf("-.-.-.-.-.-.-.-.-.-.\n-.-.-.-.-.-.-.-.-.-.\n");
	}
}

int	parse_central(t_execlist **execl, char *input, int *exit_stt)
{
	int			flag;

	ft_printf("Inside parsing.\n");
	flag = pipe_chunks(execl, input, exit_stt);
	if (flag == 1)
		flag = redir_checker(*execl, exit_stt);
	if (flag == 1)
		flag = special_char(*execl, exit_stt);
	if (flag == 1)
		flag = arg_separator(*execl, exit_stt);
	if (flag == 1)
		flag = arg_id(*execl, exit_stt);
	ft_printf("Finished parsing with flag %d\n", flag);
	return (flag);
}

/*
esta global var, bem como a exit handler, vai ter de ficar no mesmo file da main
senao não vou conseguir aceder a ele, assim ambas as funções têm acesso e
podem alterar o seu valor sem mandar pointers para a frente e p trás

usar SIGPIPE para detetar EOF conditions (ctrl D)
*/

void	sig_repeat(int num)
{
	(void)num;
	rl_replace_line("\n", 0);
	rl_redisplay();
	rl_on_new_line();
}

void	sig_repeat_two(int num)
{
	num = 2;
	global_sig = num;
}

void	global_checker(t_execlist *execl)
{
	if (global_sig == 2) //ctrl C
		free_exec(execl);
}

int	main()
{
	char		*input;
	t_execlist	*execl;
	int			exit_stt;

	global_sig = 0;
	exit_stt = 0;
	execl = NULL;
	while (1)
	{
		sig_handler_one(); //modifies the default sigactions
		//if (global_checker(execl) == 1)
			//continue;
		input = ft_read(); //with signal treatment included
		//if (global_checker(execl) == 1)
			//continue;
		sig_handler_two();
		ft_printf("Testing.\nInput = '%s';\n", input);
		if (ft_strncmp(input, "exit", 10) == 0)
		{
			ft_printf("Closing minishell...\n");
			exit(0);
		}
		if (parse_central(&execl, input, &exit_stt) == 1)
			print_exec(execl);
		exec_central(execl, &exit_stt);
		free_exec(execl);
		//else, free execl maybe, retry input
		//error_stt apenas fica gravado
		//free(input);
		//if (global_sig == 2)
			//free_exec(execl);
		//else
			//the_executor(execl, exit_stt);
	}
}

/*
◦ ctrl-C displays a new prompt on a new line.
(quero redisplay, SIGINT default)
- status: X
◦ ctrl-D exits the shell.
(quero um SIGINT, EOF end of file default)
- status: V
◦ ctrl-\ does nothing.
(quero SIG_IGN, SIGQUIT default)~
- status: V

a resposta ao meu problema de flags deve ser as masks, sigaddset e sigemptyset
while loop {
	sig 1 = close immediate
	readline
	sig 2 = free + close
	parser
	exec
}

sig 1
ctrl c - global=2, rl_newline
ctrl d - (null input), if input == NULL, exit(0)
ctrl \ - SIG_IGN

sig 1
ctrl c - global=2, rl_newline ou global=2 + checker (free)
ctrl d - global = 2, free, exit ???
ctrl \ - SIG_IGN

se bem que ainda nao sei bem como vou dar handle do ctrl D pos parser

pra ja faco assim, mas posso aprender ja sobre as masks e sets nem que seja
para o exame
opa ya. vou fazer basicamente dois sets de signal handling, faz sentido
apenas os faco manualmente e nao uso as set functions()
...
. $? no ponto 3
. testar as '' e outros parametros inteiros e frees

testing

$PATH, $HOME, $SHELL, $PWD
*/