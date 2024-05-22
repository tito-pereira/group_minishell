/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:11 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/22 18:06:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env( int *err, char **cmd, char ***envp)
{
	size_t	i;

	if (cmd[1])
	{
		ft_printf("minishell >> : env:  invalid usage\n");
		*err = 69;
	}
	else
	{
		i = 0;
		while (envp && *envp && envp[0][i])
		{
			ft_printf ("%s\n", envp[0][i]);
			i++;
		}
		*err = 0;
	}
}
