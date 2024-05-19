/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlima-fe <rlima-fe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:13 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/19 14:57:42 by rlima-fe         ###   ########.fr       */
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

void	ft_exit(int *err, char **cmd)
{
	int	code;

	if (!cmd[1])
	{
		cmd = free_db_str(cmd);
		*err = 0;
	}
	if (is_only_zeros(cmd[1]))
	{
		cmd = free_db_str(cmd);
		*err = 0;
	}
	if (!cmd[2] && is_valid_number (cmd[1]))
	{
		code = ft_atoi(cmd[1]);
		cmd = free_db_str(cmd);
		ft_printf("Exited with error code: %d", code);
		*err = code;
	}
	cmd = free_db_str(cmd);
	ft_printf("minishell >> : exit : invalid usage");
	*err = 69;
}
