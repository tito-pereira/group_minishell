/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:13 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/06/23 17:08:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (!(ft_isdigit (str[i]) || str[i] == '+'))
			return (0);
		i++;
	}
	return (1);
}

static int	is_only_zeros(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] != '0')
			return (0);
	}
	return (1);
}*/

void	ft_exit(t_execlist *execl) //int *err, char **cmd,
{
	//int	code;
	printf("inside exit builtin\n");
	printf("still here\n");
	if (execl->cmd_nmb != 1)
	{
		printf("multiple commands, no effect\n");
		return ;
	}
	/*printf("still here 2\n");
	if (!cmd[1])
	{
		cmd = free_db_str(cmd);
		*err = 0;
	}
	printf("still here 3\n");
	if (is_only_zeros(cmd[1]))
	{
		cmd = free_db_str(cmd);
		*err = 0;
	}
	printf("still here 4\n");
	if (!cmd[2] && is_valid_number (cmd[1]))
	{
		code = ft_atoi(cmd[1]);
		cmd = free_db_str(cmd);
		ft_printf("Exited with error code: %d", code);
		*err = code;
	}
	cmd = free_db_str(cmd);
	*/
	printf("still here 5\n");
	printf("previous to end is %d\n", *(execl->to_end));
	//ft_printf("minishell >> : exit : invalid usage");
	*(execl->to_end) = 1;
	printf("single command, to_end is now %d\n", *(execl->to_end));
}

/*
deve ser questao de pointers agr
e se em vez de mudar tudo, eu colocar o to_end em si como um pointer??
global?

sera que isto é um fork e o exit apenas fecha
ya. o proprio exec_action, que por si vai depois chamar a blt central,
é feito dentro dum fork

criar mais uma flag dentro do execl que a funcao exit muda para 1?
ver como se porta o bash, se só fecha no fim dos comandos, ou se fecha logo
e interrompe os outros pipes

ok o exit so funciona sozinho, sem outros pipes
if numero comandos == 1 e estiver dentro do exit, entao a flag vira 1,
e no fim da execucao da free a tudo e fecha
*/