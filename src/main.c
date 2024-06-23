/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:04 by marvin            #+#    #+#             */
/*   Updated: 2024/06/23 08:51:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_db_char(char **str) {
	int	w = 0;
	while (str[w] != NULL)
		w++;
	ft_printf("cmd_n_args has %d elements;\n", w);
	for (int i = 0; str[i] != NULL; i++) {
		ft_printf("str[%d]: %s;\n", i, str[i]);
	}
}

/*
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
}*/
///////////////////////////////////////////////

/*
-> esta global var, bem como a exit handler, vai ter de ficar no mesmo file da main
senao não vou conseguir aceder a ele, assim ambas as funções têm acesso e
podem alterar o seu valor sem mandar pointers para a frente e p trás

-> acho que o global nao vai ser para utilizar na ft_read mas sim quando sair da read para informar
que é para dar free das cenas
para ja, a unica situacao que vejo é para saber se dou free ao input buffer (escape c ctrl C) ou se é
valido e o uso (escape normal c enter)

-> voltei ao inicio de nao perceber onde usar a global. Depende de como for usar as sig handlers
tambem. Eu ia usar como "erro de status" ao sair da readline e como informacao sobre se dou free
ou nao, no entanto, ja consegui por as new_line e redisplay a dar e nao percebo onde usar
o global afinal. se calhar só se usa depois nos blocking commands
*/

char	*ft_read()
{
	char	*input;

	input = NULL;
	while (1)
	{
		//ft_printf("global_signal check: %d\n", global_sig);
		input = readline(PROMPT);
		if (input != NULL) //normal + ctrl-\ && ctrl-D ignores
		{
			add_history(input);
			return (input);
		}
		else if (input == NULL) //ctrl-D stoppage, SIGINT (ctrl-\ ignores)
			exit(0);
		/*else if (input != NULL && global_sig == 1) //ctrl-C redisplay + full buffer (clear it)
		{
			free(input);
			ft_printf("full buffer\nglobal_signal check: %d\n", global_sig);
			global_sig = 0;
			continue ;
		}
		else if (input == NULL && global_sig == 1) //ctrl-C redisplay + empty buffer (just repeat)
		{
			ft_printf("NULL buffer\nglobal_signal check: %d\n", global_sig);
			global_sig = 0;
			continue ;
		}*/
	}
}

/*
entra sempre no full buffer

sera preciso dar frees dos inputs depois de adicionar a history?
*/

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
		flag = redir_checker(*execl);
	if (flag == 1)
		flag = scope_redirs(*execl);
	if (flag == 1)
		flag = special_char(*execl);
	if (flag == 1)
		flag = arg_separator(*execl);
	if (flag == 1)
		flag = arg_id(*execl);
	//ft_printf("Finished parsing with flag %d\n", flag);//
	return (flag);
}

/*
void	init_globals(int *exit_stt, t_execlist **execl, char ***env)
{
	*exit_stt = 0;
	*execl = NULL;
	*env = create_envp();
}*/

int	main(void)
{
	char			*input;
	t_execlist		*execl;
	int				exit_stt;
	char			**env;

	exit_stt = 0;
	execl = NULL;
	env = create_envp();
	//init_globals(&exit_stt, &execl, &origin, &env);
	while (1)
	{
		sig_handler(1);
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
		sig_handler(2);
		if (parse_central(&execl, input, &exit_stt, &env) == 1)
		{
			//print_exec(execl);
			//printf("\n\n\nafter parsing\n");
			//print_db_char(execl->my_envp[0]);
			if (exec_main(execl) == 1)
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

/*
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

int global_sig = 0;
struct termios orig_termios;

// Function to reset terminal to original settings
void reset_terminal_mode()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// Function to set terminal to raw mode
void set_raw_mode()
{
    struct termios raw;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &orig_termios);

    // Make a copy of the original settings
    raw = orig_termios;

    // Input modes: no break, no CR to NL, no parity check, no strip char, no start/stop output control.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // Output modes: disable post-processing
    raw.c_oflag &= ~(OPOST);

    // Control modes: set 8-bit chars
    raw.c_cflag |= (CS8);

    // Local modes: echoing off, canonical off, no extended functions, no signal chars.
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void sig_repeat(int num)
{
    (void)num; // Unused parameter
    global_sig = 1;
    printf("\n");
    rl_replace_line("", 0);  // Clear the current line
    rl_on_new_line();        // Move cursor to a new line
    rl_redisplay();          // Redisplay the prompt
}

void	set_tty_manual()
{
	struct	termios	manual;

	tcgetattr(STDIN_FILENO, &manual);
    manual.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    manual.c_oflag &= ~(OPOST);
    manual.c_cflag |= (CS8);
	manual.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, &manual);
}

void	sig_repeat(int num)
{
	struct termios origin;

	(void)num;
	tcgetattr(STDIN_FILENO, &origin);
	set_tty_manual(); //set to manual
	global_sig = 1;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	tcsetattr(STDIN_FILENO, TCSANOW, &origin); //reset to origins
}
*/