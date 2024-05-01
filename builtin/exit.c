/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:13 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/04/30 13:03:03 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	exit_handler(const char *msg, int code)
{
	if (code && code != 127)
		ft_putstr_fd("ERROR\n", 2);
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	if (!code && !msg)
		ft_putendl_fd("See you soon, human!", 1);
	rl_clear_history();
	exit (code);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	x;

	i = 0;
	n = 1;
	x = 0;
	while ((str[i] == 32) || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+' )
	{
		if (str[i] == '-' )
			n = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' )
	{
		x = (x * 10) + (str[i] - 48);
		i++;
	}
	return (x * n);
}

int	is_only_zeros(char	*nbr)
{
	int	i;

	i = -1;
	while (nbr[++i])
	{
		if (nbr[i] != '0')
			return (0);
	}
	return (1);
}

int	is_valid(char *nbr)
{
	int	i;

	i = 0;
	if (nbr[0] == '+')
		i++;
	while (nbr[i])
	{
		if (!ft_isdigit(nbr[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **cmd)
{
	int	code;

	if (!cmd[1])
	{
		free_mat(cmd);
		exit_handler(NULL, 0);
	}
	if (is_only_zeros(cmd[1]))
	{
		free_mat(cmd);
		exit_handler(NULL, 0);
	}
	if (!cmd[2] && is_valid(cmd[1]))
	{
		code = ft_atoi(cmd[1]);
		free_mat(cmd);
		exit_handler("exited with code: ", code);
	}
	cmd = free_mat(cmd);
	exit_handler("-" NPROMPT ": exit: too many arguments", 2);
}
