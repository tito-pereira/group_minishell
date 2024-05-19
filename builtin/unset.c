/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:19 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/19 14:48:03 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		envp[i] = free_str(envp[i]);
		envp[i] = envp[i + 1];
		i++;
		while (envp[i])
		{
			envp[i] = envp[i + 1];
			i++;
		}
		envp[i] = NULL;
	}
	temp_var = free_str(temp_var);
}

void	ft_unset(int *err, char **cmd, char ***envp)
{
	while (*(++cmd))
		remove_var(*cmd, envp);
	*err = 0;
}
