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

void	sig_handler(int mode)
{
	struct	sigaction	sa_repeat;
	struct	sigaction	sa_ign;
	struct	sigaction	sa_dfl;

	sa_repeat.sa_handler = &sig_repeat;
	sa_ign.sa_handler = SIG_IGN;
	sa_dfl.sa_handler = SIG_DFL;
	if (mode == 1)
	{
		sigaction(SIGINT, &sa_repeat, NULL);
		sigaction(SIGQUIT, &sa_ign, NULL);
	}
	else if (mode == 2)
		sigaction(SIGINT, &sa_dfl, NULL);
	else if (mode == 3)
		sigaction(SIGINT, &sa_ign, NULL);
}

/*

(vanilla, SIGINT)
.(?) ctrl C, empty prompt, repeat (vanilla: )
.(?) ctrl C, some lines, repeat (vanilla: )
.(V) ctrl C, blocking commands, (repeat?)
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
.(?) ctrl \, blocking commands, nothing
◦ ctrl-\ does nothing.
(new, SIG_IGN)

-------------------------
sig 1 - pre readline
(X) ctrl c - redisplay    , --------------
(V) ctrl d -------------- , input == NULL && global_sig == 0
(X) ctrl \ - SIG_IGN      , --------------

sig 2 - pos readline, pre command
(X) ctrl c 
(X) ctrl d -------------- (outro mecanismo)
(X) ctrl \ -------------- 

como nao consigo usar signal handling em ctrl-D
vamos supor entao que nao consigo controlar ctrl-D em buffers de input cheios
no blocking command eu so posso lidar com o returno do execve()

o sinal é mudado no parent process.
nao consigo controlar o cat dentro do execve, so consigo que ele
herde coisas minhas. mas qualquer influencia do sinal é no
parent process e nao no cat dentro do execve

mudo o ctrl-C para default antes do execve, e mudo o ctrl-C para IGN right after?
para nao haver conflitos de sinais entre o parent process e todos os forked child
processes que vou criar
------------------------------
-> ctrl C ainda nao da propriamente buffer vazio, retorna sempre null
-> ctrl-D c blocking command precisa de tratamento (provavelmente usando error status, em vez de
continuar no loop do minishell, da free a tudo e sai)


c1r8s6% cat		(empty ctrl-D)
c1r8s6% cat		(full ctrl-D)
cwenocweno%                                                                                             
c1r8s6% cat		(empty ctrl-C)
^C
c1r8s6% cat		(full ctrl-C)
cweochoi^C

em relacao ao porque de o sinal global sentir quase imperativo ou obrigatorio, é
muito simplesmente porque o mecanismo habitual é transferencia de informacao
direta entre funcoes atraves de pointers ou returns, e a variavel global existe quando
a transmissao de info entre funcoes dentro do mesmo processo se torna impossivel
Portanto, eu sei que o global_sig vai ser para comunicar entre entre as handlers e as
main functions. Agora onde e para que, ainda nao sei

outra questao ta a ser aquilo do canonical vs manual mode de terminal input,
quando usar qual, e tudo isto apenas para retirar o ^C de aparecer.
Vai ser normal aparecer, em maior parte dos casos, os sinais no display do ecra uma vez
que o meu minishell corre da mesma maneira que um bash terminal normal, em batch mode,
que significa receber input linha a linha em ve de caracter a caracter em tempo real
(para mais info pesquisa na net)
*/