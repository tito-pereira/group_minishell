/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:08 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 00:38:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	ft_echo(t_mini *ms, char **cmd)
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
	ms->error = 0;
}
