/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:21 by marvin            #+#    #+#             */
/*   Updated: 2024/05/13 13:26:32 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <linux/limits.h>
# include "./mylib/mylib.h"

# define ENV_VAR    __environ
# define PROMPT		">> minishell: "
# define NPROMPT	"\n>> minishell: "

typedef struct s_chunk {
	char	*infile; // (2) redir_checker
	int		heredoc; // (2) redir_checker
	char	*delimiter; // (2) redir_checker
	char	*outfile; // (2) redir_checker
	int		append; // (2) redir_checker
	char	*og; // (1) parse_execl
	char	**cmd_n_args; // (4) arg_separator
	char	*path; // (5) arg_id
	int		inpipe; // (5) arg_id
	int		inpfd; // executor
	int		outpipe; //acho que já não é necessário
	int		outpfd; // executor
	int		blt; // (5) arg_id
}	t_chunk;

typedef struct s_execlist {
	t_chunk	**chunk;
	int		cmd_nmb;
	int		pipe_nmb;
	char	**my_envp;
	int		valid_cmds;
	int		*pipe_loc;
}	t_execlist;
/*
valid_cmds && cmd_nmb vai dar ao mesmo
*/

typedef struct s_mini
{
    char    *input;
    char    *prompt;
    char    **envp;
    char    error;
    char    exit;
}   t_mini;

// SIGNAL HANDLING
void	sig_handler_one(void);
void	sig_handler_two(void);
void	sig_repeat(int num);
//void	sig_f_exit(int num);
//void	global_checker(t_execlist *execl);

// PARSER MAIN
int		parse_central(t_execlist **execl, char *input, int *exit_stt);
int		pipe_chunks(t_execlist **execl, char *input, int *exit_stt); // (1)
int		redir_checker(t_execlist *execl, int *exit_stt); // (2)
int		scope_redirs(t_execlist *execl, int *exit_stt); // (3)
int		special_char(t_execlist *execl, int *exit_stt); // (4)
int		arg_separator(t_execlist *execl, int *exit_stt); // (5)
int		arg_id(t_execlist *execl, int *exit_stt); // (6)

// PARSER SUPPORT
int		pipe_counter(char *input, t_execlist *execl); // (1)
int		input_redir(t_chunk *chunk, int *i); // (2)
int		output_redir(t_chunk *chunk, int *i); // (2)
char	*new_chnk(char *spec, char *old, int a, int b); // (3)
int		cmd_separator(t_chunk *chunk); // (4)
void	add_arg(t_chunk *chunk, char **str); // (4)
int		chunk_id(t_chunk *chunk, char *prog, int opt); // (5)

// OTHER
char	**create_envp(void);
void	close_pipes(t_execlist *execl, int **fd, int i, int rel, int non_rel);

// EXECUTOR
int		exec_main(t_execlist *execl, int *error_stt);
void	exec_loop(t_execlist *execl, int **fd, int **redir, char ***exec_str);
void	exec_input(t_execlist *execl, int **fd, int **redir, int i);
void	exec_output(t_execlist *execl, int **fd, int **redir, int i);

// FREE
char	*free_str(char *str);
char	**free_db_str(char **str);
void	free_chunk(t_chunk *chunk);
void	free_exec(t_execlist *exec);

// BUILT-IN
void	ft_cd(t_mini *ms, char **cmd, char **env);
void	ft_echo(t_mini *ms, char **cmd);
void	ft_env(t_mini *ms, char **cmd, char **envp);
void	ft_exit(char **cmd);
void	ft_export(t_mini *ms, char **cmd, char **envp);
void    ft_pwd(t_mini *ms, char **cmd, char **envp);
void	ft_unset(t_mini *ms, char **cmd, char **envp);

#endif