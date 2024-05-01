/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:35:51 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/01 13:50:02 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin(char *arg)
{
	ft_printf("Checking if builtin or not:\n");
	if (ft_strncmp("echo", arg, 256) == 0
		|| ft_strncmp("cd", arg, 256) == 0
		|| ft_strncmp("pwd", arg, 256) == 0
		|| ft_strncmp("export", arg, 256) == 0
		|| ft_strncmp("unset", arg, 256) == 0
		|| ft_strncmp("env", arg, 256) == 0
		|| ft_strncmp("exit", arg, 256) == 0)
	{
		ft_printf("Builtin confirmed.\n");
		return (1);
	}
	ft_printf("Not builtin.\n");
	return (0);
}

int	arg_id(t_execlist *execl, int *exit_stt)
{
	int	i;
	int	r;

	i = -1;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (5): arg_id;\n");
	while (execl->chunk[++i] != NULL)
	{
		ft_printf("Getting command id from chunk n°%d:\n", i);
		if (i >= 1)
			execl->chunk[i]->inpipe = 1;
		execl->chunk[i]->blt = check_builtin(execl->chunk[i]->cmd_n_args[0]);
		if (execl->chunk[i]->blt == 1)
			r = chunk_id(execl->chunk[i], "builtft", 1);
		else if (execl->chunk[i]->blt == 0)
			r = chunk_id(execl->chunk[i], NULL, 2);
		if (r == 0)
		{
			perror("Command path finding error");
			*exit_stt = 127;
			return(0);
		}
	}
	ft_printf("Finished the arg id parsing.\n");
	return(1);
}

/*
so tou a usar builtins por isso o erro esta na parte dos builtins


if (BUILTIN) , flag == 1
arg[0] = path/builtft
arg[1] = (char *)cmd_n_args;

else (terminal) , flag == 0
path_n_args || modify cmd_n_args
arg[0] = path/terminal
arg[1] = arg normal

ADD CHUNK_PATH
por default,
execl->chunk[i]->path = NULL;
senao, mudar apenas nos builtins;

vou ter que retirar aquela condicao de erro porque vou aceitar todos os
comandos e so verifico os erros no executor
OU MANTENHO?
*/