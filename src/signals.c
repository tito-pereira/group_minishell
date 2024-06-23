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

void	sig_repeat(int num)
{
	(void)num;
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
	sa_dfl.sa_handler = SIG_DFL; //return to default?
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
.(V) ctrl C, empty prompt, repeat (vanilla: )
.(V) ctrl C, some lines, repeat (vanilla: )
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
.(V) ctrl \, blocking commands, nothing
◦ ctrl-\ does nothing.
(new, SIG_IGN)

-------------------------

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

posso usar o sighandler para informar o meu executor, pos execucao, se recebeu ctrl-C?
tamben nao tenho acesso ao retorno do execve
------------------------------

-> ctrl-D c blocking command precisa de tratamento (provavelmente usando error status, em vez de
continuar no loop do minishell, da free a tudo e sai)

unica coisa que sei é, nao posso deixar o default ctrl-D para blocking commands
para readline até funciona bem. tem que funcionar como SIGINT. o default SIGINT, em bash, fecha
logo tudo mesmo estando em blocking commands (nope, apenas fecha o cat)

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


comportamentos muito parecidos honestamente

MUDAR NA READLINE DO HEREDOC
fazer o mecanismo que faco na read mas desta vez quando uso readline ao ler o input
para o heredoc
vai ser equivalente a ter recebido esse sinal
*/