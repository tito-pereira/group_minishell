/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:05:50 by marvin            #+#    #+#             */
/*   Updated: 2024/04/04 19:05:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
void	sigint_hdhandler(int signo)
{
	if (signo == SIGINT)
	{
		g_signo = 130;
		write(1, "\n", 1);
		exit(g_signo);
	}
}

*/

void	sig_repeat(int num)
{
	(void)num;
	g_sig += SIGINT;
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handler(int mode)
{
	struct	sigaction	sa_repeat;
	struct	sigaction	sa_ign;
	struct	sigaction	sa_dfl;

	sa_repeat.sa_handler = &sig_repeat;
	sa_ign.sa_handler = SIG_IGN;
	sa_dfl.sa_handler = SIG_DFL;
	if (mode == 1) //prompt
	{
		sigaction(SIGINT, &sa_repeat, NULL);
		sigaction(SIGQUIT, &sa_ign, NULL);
	}
	else if (mode == 2) //heredoc ???
		sigaction(SIGINT, &sa_dfl, NULL);
	else if (mode == 3) //pre executor ???
		sigaction(SIGINT, &sa_ign, NULL);
}

/*

(vanilla, SIGINT)
.(V) ctrl C, empty prompt, repeat (vanilla: )
.(V) ctrl C, some lines, repeat (vanilla: )
.(X) ctrl C, heredoc, repeat (vanilla: )
.(V) ctrl C, blocking commands, (repeat?)
◦ ctrl-C displays a new prompt on a new line.
(new, repeat)

(vanilla, EOF)
.(V) ctrl D, empty prompt, quit (vanilla: NULL input && repeat)
.(V) ctrl D, full prompt, nothing (vanilla: nothing)
.(X) ctrl D, heredoc, repeat (vanilla: )
.(-) ctrl D, blocking commands, (quit/nothing) (vanilla: NULL input && repeat)
◦ ctrl-D exits the shell.
(new, SIGINT)

(vanilla, SIGQUIT)
.(V) ctrl \, empty prompt, ignore
.(V) ctrl \, full prompt, ignore
.(V) ctrl \, heredoc, ignore
.(V) ctrl \, blocking commands, nothing
◦ ctrl-\ does nothing.
(new, SIG_IGN)

default
c1r5s3% cat (ctrl-C, SIGINT)
^C
c1r5s3% cat (ctrl-D, EOF)
c1r5s3% 
c1r5s3% cat (ctrl-C, SIGINT, 1 clique)
stuff^C
c1r5s3% cat (ctrl-D, EOF, 2 cliques)
stuffstuff%
c1r5s3% 

basicamente tudo o que envolva blocking commands vai ser default, nao
ha grande maneira de controlar porque eles dao reset aos sighandlers
o que podemos fazer é registar a global var no parent process e após o
fecho do child process lidar com o sinal recebido

teste triplo ecra:
- empty prompt
- full prompt
- empty heredoc
- full heredoc
- blocking command

- nao acho que o sigpipe seja necessário, apenas acrescenta uma mensagem extra
- hdhandler
- handler fork
*/

/*
void	sigpipe_handler(int signo)
{
	(void)signo;
	printf("Received SIGPIPE signal. Pipe closed by child process.\n");
	exit(EXIT_FAILURE);
}

void	sigint_hdhandler(int signo)
{
	if (signo == SIGINT)
	{
		g_signo = 130;
		write(1, "\n", 1);
		exit(g_signo);
	}
}

void	sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signo = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sighandler_fork(int signo)
{
	if (signo == SIGINT || signo == SIGQUIT)
		g_signo = 128 + signo;
}

void	set_signals(void)
{
	g_signo = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
*/