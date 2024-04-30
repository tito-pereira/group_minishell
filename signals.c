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
void	sig_f_exit(int num)
{
	num = 1;
	global_sig = num;
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

/*

ja sei
usar as rl_newline etc etc
se eu deixar default, ctrl C fecha logo na readline
tenho que alterar algo mas so posso comunicar com uma global
por isso, se eu apenas mudar um int, a readline permanece em execucao
e eu posso apenas usar as redisplay, novo prompt, new line, etc

e tambem acho q nem preciso do sig_handler 2, uso apenas 1
*/