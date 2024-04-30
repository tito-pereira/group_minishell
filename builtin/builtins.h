/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:55:14 by tibarbos          #+#    #+#             */
/*   Updated: 2024/04/30 13:07:02 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
#define BUILTINS_H

#include "../minishell.h"

void	ft_cd(t_mini *ms, char **cmd, char **env);
void	ft_echo(t_mini *ms, char **cmd);
void	ft_env(t_mini *ms, char **cmd, char **envp);
void	ft_exit(t_mini *ms, char **cmd);
void	ft_export(t_mini *ms, char **cmd, char **envp);
void    ft_pwd(t_mini *ms, char **cmd, char **envp);
void	ft_unset(t_mini *ms, char **cmd, char **envp);

#endif