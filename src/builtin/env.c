/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:11 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/06/30 22:04:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_env( int *err, char **cmd, char ***envp)
{
	size_t	i;

	//printf("inside ft env\n");
	(void)err;
	if (cmd[1])
	{
		ft_printf("minishell >> : env:  invalid usage\n");
		//*err = 69;
	}
	else
	{
		i = 0;
		while (envp && *envp && envp[0][i])
		{
			ft_printf ("%s\n", envp[0][i]);
			i++;
		}
		//*err = 0;
	}
	//printf("leaving env\n");
}
