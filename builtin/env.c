/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:11 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/17 15:12:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(int *err, char **cmd, char **envp)
{
	int	i;

	if (!cmd[1])
	{
		i = 0;
		while (envp && *envp && envp[i][0])
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
		*err = 0;
	}
	else
	{
		ft_putstr_fd("Bad usage, burro.", 2);
		*err = 69;
	}
}
