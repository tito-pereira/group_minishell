/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:06:02 by rlima-fe          #+#    #+#             */
/*   Updated: 2024/05/11 11:17:15 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execbi(t_mini *ms, char **cmd, char **envp)
{
    if (!ms || !cmd || !envp)
        return (-1);
    if (!ft_strncmp(*cmd, "echo", 5))
        ft_echo(ms, cmd);
    else if (!ft_strncmp(*cmd, "cd", 3))
        ft_cd(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "pwd", 4))
        ft_pwd(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "export", 7))
        ft_export(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "unset", 6))
        ft_unset(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "env", 4))
        ft_env(ms, cmd, envp);
    else if (!ft_strncmp(*cmd, "exit", 5))
        ft_exit(cmd);
    else
        return (0);
    return (1);
}
