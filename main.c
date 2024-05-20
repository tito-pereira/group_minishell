/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:04 by marvin            #+#    #+#             */
/*   Updated: 2024/05/20 14:26:28 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (execl->chunk[i]->blt == 1)
			ft_printf("PATH: %s;\n", execl->chunk[i]->path);
		else
			ft_printf("PATH: NULL;\n");;
		ft_printf("Heredoc: %d;\nAppend: %d;\n", execl->chunk[i]->heredoc, execl->chunk[i]->append);
		if (execl->chunk[i]->infile)
			ft_printf("infile: %s;\n", execl->chunk[i]->infile);
		else
			ft_printf("infile: NULL\n");
		if (execl->chunk[i]->outfile)
			ft_printf("outfile: %s;\n", execl->chunk[i]->outfile);
		else
			ft_printf("outfile: NULL\n");
		ft_printf("-.-.-.-.-.-.-.-.-.-.\n-.-.-.-.-.-.-.-.-.-.\n");
	}
}
///////////////////////////////////////////////

int	global_sig;
// 0 neutro e/ou ctrl D
// 1 para redisplay; (ctrl C)

/*
esta global var, bem como a exit handler, vai ter de ficar no mesmo file da main
senao não vou conseguir aceder a ele, assim ambas as funções têm acesso e
podem alterar o seu valor sem mandar pointers para a frente e p trás

ach que o global nao vai ser para utilizar na ft_read mas sim quando sair da read para informar
que é para dar free das cenas
para ja, a unica situacao que vejo é para saber se dou free ao input buffer (escape c ctrl C) ou se é valido
e o uso (escape normal c enter)
*/

char	*ft_read()
{
	char	*input;

	input = NULL;
	while (1)
	{
		ft_printf("global_signal check: %d\n", global_sig);
		input = readline(PROMPT);
		if (input != NULL && global_sig == 0) //normal + ctrl-\ && ctrl-D ignores
		{
			add_history(input);
			return (input);
		}
		else if (input == NULL && global_sig == 0) //ctrl-D stoppage, SIGINT (ctrl-\ ignores)
			exit(0);
		else if (input != NULL && global_sig == 1) //ctrl-C redisplay + full buffer (clear it)
		{
			free(input);
			ft_printf("global_signal check: %d\n", global_sig);
			global_sig = 0;
			continue ;
		}
		else if (input == NULL && global_sig == 1) //ctrl-C redisplay + empty buffer (just repeat)
		{
			ft_printf("global_signal check: %d\n", global_sig);
			global_sig = 0;
			continue ;
		}
	}
}

/*
sera preciso dar frees dos inputs depois de adicionar a history?

Function: int rl_on_new_line ()
Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline.

Function: int rl_redisplay ()
Change what's displayed on the screen to reflect the current contents of rl_line_buffer.
*/

// super clunky e estranho
void	sig_repeat(int num)
{
	(void)num;
	global_sig = 1;
	//rl_replace_line(PROMPT, 0);
	printf("\n");
	rl_on_new_line();
	//rl_replace_line(PROMPT, 0);
	rl_redisplay();
	//rl_replace_line("", 0);
}

char	**create_envp(void)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (__environ[i] != NULL)
		i++;
	my_envp = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while (__environ[++i] != NULL)
		my_envp[i] = ft_strdup(__environ[i]);
	my_envp[i] = NULL;
	return(my_envp);
}

int	null_input(char *input, int *exit_stt)
{
	int	i;

	i = -1;
	if (input[0] == '\0')
	{
		perror("Empty input");
		*exit_stt = 1;
		return (0);
	}
	while (input[++i])
	{
		if (input[i] != ' ' && input[i] != '	')
			return (1);
	}
	perror("Empty input");
	*exit_stt = 1;
	return (0);
}

int	parse_central(t_execlist **execl, char *input, int *exit_stt, char ***env)
{
	int			flag;

	//ft_printf("Inside parsing.\n");//
	flag = null_input(input, exit_stt);
	if (flag == 1)
		flag = pipe_chunks(execl, input, exit_stt, env);
	if (flag == 1)
		flag = redir_checker(*execl, exit_stt);
	if (flag == 1)
		flag = scope_redirs(*execl, exit_stt);
	if (flag == 1)
		flag = special_char(*execl, exit_stt);
	if (flag == 1)
		flag = arg_separator(*execl, exit_stt);
	if (flag == 1)
		flag = arg_id(*execl, exit_stt);
	//ft_printf("Finished parsing with flag %d\n", flag);//
	return (flag);
}

int	main()
{
	char		*input;
	t_execlist	*execl;
	int			exit_stt;
	char		**env;

	global_sig = 0;
	exit_stt = 0;
	execl = NULL;
	env = create_envp();
	while (1)
	{
		sig_handler_one();
		//if (global_checker(execl) == 1)
			//continue;
		//ft_printf("\n\n\nbegining env\n");
		//print_db_char(env);
		input = ft_read(); //with signal treatment included
		//if (global_checker(execl) == 1)
			//continue;
		// sig_handler_two();
		//ft_printf("Testing.\nInput = '%s';\n", input);//
		///
		/*if (ft_strncmp(input, "exit", 10) == 0)
		{
			ft_printf("Closing minishell...\n");
			exit(0);
		}*/
		if (parse_central(&execl, input, &exit_stt, &env) == 1)
		{
			print_exec(execl);
			//printf("\n\n\nafter parsing\n");
			//print_db_char(execl->my_envp[0]);
			if (exec_main(execl, &exit_stt) == 1)
			{
				//if check_change() == 1
				env = execl->my_envp[0];
				exit_stt = 0;
			}
		}
		if (execl)
			free_exec(execl);
		//ft_printf("\n\n\nafter free env\n");
		//print_db_char(env);
		//free_db_str(env);
		//env = execl->my_envp[0];
		/////////////////////
		//free(input);
		//if (global_sig == 2)
			//free_exec(execl);
		//else
			//the_executor(execl, exit_stt);
	}
}
