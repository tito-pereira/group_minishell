/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:55:14 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/01 14:26:12 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
#define BUILTINS_H

#include "../minishell.h"

typedef struct s_mini
{
    char    *input;
    char    *prompt;
    char    **envp;
    //t_token *token;
    char    error;
    char    exit;
}   t_mini;

void	ft_cd(t_mini *ms, char **cmd, char **env);
void	ft_echo(t_mini *ms, char **cmd);
void	ft_env(t_mini *ms, char **cmd, char **envp);
void	ft_exit(t_mini *ms, char **cmd);
void	ft_export(t_mini *ms, char **cmd, char **envp);
void    ft_pwd(t_mini *ms, char **cmd, char **envp);
void	ft_unset(t_mini *ms, char **cmd, char **envp);

#endif