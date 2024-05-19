/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_central.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlima-fe <rlima-fe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/19 15:23:44 by rlima-fe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	blt_central(t_execlist *execl, int i, char **exec_str, int *err)
{
	ft_printf("IM INSIDE BUILTFT\n");
	if (ft_strncmp(exec_str[0], "cd", 3) == 0)
		ft_cd(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
	else if (ft_strncmp(exec_str[0], "echo", 5) == 0)
		ft_echo(err, execl->chunk[i]->cmd_n_args);
	else if (ft_strncmp(exec_str[0], "env", 4) == 0)
		ft_env(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "exit", 5) == 0)
		ft_exit(err, execl->chunk[i]->cmd_n_args);
    else if (ft_strncmp(exec_str[0], "export", 7) == 0)
		ft_export(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "pwd", 4) == 0)
		ft_pwd(err, execl->chunk[i]->cmd_n_args, *(execl->my_envp));
    else if (ft_strncmp(exec_str[0], "unset", 6) == 0)
		ft_unset(err, execl->chunk[i]->cmd_n_args, execl->my_envp);
}
