/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:17 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 11:15:53 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_pwd(t_mini *ms, char **cmd, char **envp)
{
	int	i;

	if (!cmd[1])
	{
		i = 0;
		while (envp && ft_strncmp(envp[i], "PWD=", 4))
			i++;
		if (envp && envp[i])
			ft_printf("%s", envp[i] + 4);
	}
	else
	{

		ft_putstr_fd("Bad usage, burro.", 2);
		ms->error = 69;
	}
}
