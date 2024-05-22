/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:17 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/22 18:07:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_pwd(int *err, char **cmd, char **envp)
{
	size_t	i;

	if (!cmd[1])
	{
		i = 0;
		while (envp && ft_strncmp (envp[i], "PWD=", 4))
			i++;
		if (envp && envp[i])
			ft_printf ("%s\n", envp[i] + 4);
	}
	else
	{
		ft_printf("minishell >> : pwd : invalid usage\n");
		*err = 69;
	}
	*err = 0;
}
