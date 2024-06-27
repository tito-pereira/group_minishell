/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 01:45:13 by marvin            #+#    #+#             */
/*   Updated: 2024/06/27 01:45:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
}

void	ft_exit(int *err, char **cmd, t_execlist *execl)
{
	int	code;
	
	/*if (execl->cmd_nmb != 1) //dentro de pipes
	{
		ft_printf("minishell >> : exit : invalid usage\n");
		return ;
	}*/
	if (!cmd[1] && execl) //sem args, válido
	{
		//cmd = free_db_str(cmd);
		//*err = 0;
		free_exec(execl);
		exit(0);
	}
	if (!cmd[2] && is_only_zeros(cmd[1]) == 1 && execl) //apenas zeros, válido
	{
		//cmd = free_db_str(cmd);
		//*err = 0;
		free_exec(execl);
		exit(0);
	}
	if (!cmd[2] && is_valid_number(cmd[1]) == 1 && execl) //1 arg numero valido
	{
		//cmd = free_db_str(cmd);
		//*err = code;
		code = ft_atoi(cmd[1]);
		free_exec(execl);
		ft_printf("Exited with error code: %d\n", code);
		exit(code);
	}
	//cmd = free_db_str(cmd);
	ft_printf("minishell >> : exit : invalid usage\n");
}

/*
acho que nao preciso deste code aqui, faço free + exit direto
com o numero correspondente

fora:
- exit command called (strcmp)
- ausencia de pipes (cmd nmb == 1)

dentro:
- so 1 argumento
- argumento valido (digit)

- tirar to_end
*/