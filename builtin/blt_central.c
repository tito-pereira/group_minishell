/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_central.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:01:23 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/21 15:41:40 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	blt_central(t_execlist *execl, int i, char **exec_str)
{
	//ft_printf("IM INSIDE BUILTFT\n");
	if (ft_strncmp(exec_str[0], "cd", 3) == 0)
		ft_cd(execl->exit_stt, execl->chunk[i]->cmd_n_args, execl->my_envp);
	else if (ft_strncmp(exec_str[0], "echo", 5) == 0)
		ft_echo(execl->exit_stt, execl->chunk[i]->cmd_n_args);
	else if (ft_strncmp(exec_str[0], "env", 4) == 0)
		ft_env(execl->exit_stt, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "exit", 5) == 0)
		ft_exit(execl->exit_stt, execl->chunk[i]->cmd_n_args);
    else if (ft_strncmp(exec_str[0], "export", 7) == 0)
		ft_export(execl->exit_stt, execl->chunk[i]->cmd_n_args, execl->my_envp);
    else if (ft_strncmp(exec_str[0], "pwd", 4) == 0)
		ft_pwd(execl->exit_stt, execl->chunk[i]->cmd_n_args, *(execl->my_envp));
    else if (ft_strncmp(exec_str[0], "unset", 6) == 0)
		ft_unset(execl->exit_stt, execl->chunk[i]->cmd_n_args, execl->my_envp);
	//ft_printf("\n\n\ndentro blt central env\n");
	//print_db_char(execl->my_envp[0]);
}
