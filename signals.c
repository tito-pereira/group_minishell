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

#include "minishell.h"

/*
void	sig_repeat_two(int num)
{
	num = 2;
	global_sig = num;
}

void	global_checker(t_execlist *execl)
{
	if (global_sig == 2) //ctrl C
		free_exec(execl);
}*/

void	sig_handler_one(void)
{
	struct	sigaction	sa_repeat;
	struct	sigaction	sa_ign;

	sa_repeat.sa_handler = &sig_repeat;
	sa_ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_repeat, NULL);
	sigaction(SIGQUIT, &sa_ign, NULL);
}

/*

(vanilla, SIGINT)
.( ) ctrl C, empty prompt, repeat (vanilla: )
.( ) ctrl C, some lines, repeat (vanilla: )
.( ) ctrl C, blocking commands, (repeat?)
◦ ctrl-C displays a new prompt on a new line.
(new, repeat)

(vanilla, EOF)
.(V) ctrl D, empty prompt, quit (vanilla: NULL input && repeat)
.(V) ctrl D, full prompt, nothing (vanilla: nothing)
.(-) ctrl D, blocking commands, (quit/nothing) (vanilla: NULL input && repeat)
◦ ctrl-D exits the shell.
(new, SIGINT)

(vanilla, SIGQUIT)
.(V) ctrl \, empty prompt, nothing
.(V) ctrl \, full prompt, nothing
.(-) ctrl \, blocking commands, nothing
◦ ctrl-\ does nothing.
(new, SIG_IGN)

-------------------------
sig 1 - pre readline
(X) ctrl c - global=2, rl_newline
(V) ctrl d -------------- input == NULL && global_sig == 0
(X) ctrl \ - SIG_IGN

sig 2 - pos readline, pre command
(X) ctrl c - volta a sigint ou quit n sei
(X) ctrl d -------------- (outro mecanismo)
(X) ctrl \ -------------- 

e se eu usar um EOF no ctrl-C? mas depois nao consigo set a global_var em 1
------------------------------

-> ainda nao sei como tratar a ctrl C em nenhum caso
-> ctrl-D c blocking command precisa de tratamento (provavelmente usando error status, em vez de
continuar no loop do minishell, da free a tudo e sai)
-> ctrl-\ in blocking commands imprime o ^\



while loop {
	sig 1 = close immediate (porque ainda nao dei malloc de nada)
	readline
	after readline
	sig 2 = free + close
	parser
	exec
	blocking command
}

void	sig_handler_two(void)
{
	//struct	sigaction	sa_f_exit;
	struct	sigaction	sa_repeat;
	struct	sigaction	sa_ign;

	//sa_f_exit.sa_handler = &sig_f_exit;
	sa_repeat.sa_handler = &sig_repeat;
	sa_ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_repeat, NULL); //ctrl C
	//sigaction(EOF, &sa_f_exit, NULL); //ctrl D
	sigaction(SIGQUIT, &sa_ign, NULL); //ctrl barra
}


a resposta ao meu problema de flags deve ser as masks, sigaddset e sigemptyset
ja sei
usar as rl_newline etc etc
se eu deixar default, ctrl C fecha logo na readline
tenho que alterar algo mas so posso comunicar com uma global
por isso, se eu apenas mudar um int, a readline permanece em execucao
e eu posso apenas usar as redisplay, novo prompt, new line, etc

em relacao ao porque de o sinal global sentir quase imperativo ou obrigatorio, é
muito simplesmente porque o mecanismo habitual é transferencia de informacao
direta entre funcoes atraves de pointers ou returns, e a variavel global existe quando
a transmissao de info entre funcoes dentro do mesmo processo se torna impossivel
*/