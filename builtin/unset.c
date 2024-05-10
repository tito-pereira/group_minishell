/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:19 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 00:38:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	remove_var(char *var, char **envp)
{
	int	i;
	char	*temp_var;

	if (!var || !envp)
		return ;
	temp_var = ft_strjoin(var, "=");
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], temp_var, ft_strlen(temp_var)))
		i++;
	if (envp && envp[i])
	{
		envp[i] = free_ptr(envp[i]);
		envp[i] = envp[i + 1];
		i++;
		while (envp[i])
		{
			envp[i] = envp[i + 1];
			i++;
		}
		envp[i] = NULL;
	}
	temp_var = free_ptr(temp_var);
}

void	ft_unset(t_mini *ms, char **cmd, char **envp)
{
	while (*(++cmd))
		remove_var(*cmd, envp);
	ms->error = 0;
}
