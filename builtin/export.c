/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:15 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 11:47:10 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_var_pos(char *var, char **envp)
{
	char	*temp_var;
	int		var_len;
	int		var_pos;

	var_len = ft_strchr(var, '=') - var;
	temp_var = ft_calloc(var_len + 2, sizeof(char));
	ft_strlcpy (temp_var, var, var_len + 2);
	temp_var[var_len] = '=';
	temp_var[var_len + 1] = '\0';
	var_pos = 0;
	while (envp[var_pos] && ft_strncmp(temp_var, envp[var_pos], var_len + 1))
		var_pos++;
	free_str(&temp_var);
	return (var_pos);
}

void	update_var(char *var, int var_pos, char **envp)
{
	char	**temp_envp;

	if (!envp[var_pos])
	{
		temp_envp = ft_calloc(var_pos + 2, sizeof(char *));
		temp_envp[var_pos] = var;
		while (var_pos--)
			temp_envp[var_pos] = envp[var_pos];
		free_db_str(&envp);
		envp = temp_envp;
	}
	else
	{
		free_str(&(envp[var_pos]));
		envp[var_pos] = ft_strdup(var);
	}
}

int	valid_var(char *var)
{
	int	i;
	int	res;

	res = 1;
	i = 0;
	if (!var || var[0] == '=')
		res = 0;
	if (isdigit(var[0]))
		res = 0;
	while (var[i] && var[i] != '=')
	{
		if (var[i] != '_' && !ft_isalnum(var[i]))
			res = 0;
		i++;
	}
	return (res);
}

void	ft_export(t_mini *ms, char **cmd, char **envp)
{
	int	i;
	int	var_pos;

	if (!cmd[1])
		ft_env(ms, cmd, envp);
	i = 1;
	while (cmd[i])
	{
		if (valid_var(cmd[i]) && ft_strchr(cmd[i], '='))
		{
			ms->error = 0;
			var_pos = get_var_pos(cmd[i], envp);
			update_var(cmd[i], var_pos, envp);
		}
		else if (!valid_var(cmd[i]))
		{
			ft_putstr_fd(NPROMPT": export: ", 2);
			ft_putstr_fd(cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			ms->error = 69;
		}
		else
			ms->error = 0;
		i++;
	}
}
