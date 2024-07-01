/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:08 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/06/30 22:31:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_echo(int *err, char **cmd)
{
	int newline;
	int i;
	int j;

	newline = 1;
	i = 1;
	while (cmd && cmd[i] && !ft_strncmp(cmd[i], "-n", 2))
	{
		j = 2;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] != '\0')
			break;
		newline = 0;
		i++;
	}
	while (cmd[i])
	{
		ft_printf("%s", cmd[i]);
		if (cmd[++i])
			ft_putchar_fd(' ', 1);
	}
	if (newline)
		ft_printf("\n");
	//*err = 0;
	(void)err;
}

/*
nao esta a dar esta merda
eu admito que nao receba input redirs, mas output redir e
imprimir os args é o minimo que deveria fazer aqui
*/