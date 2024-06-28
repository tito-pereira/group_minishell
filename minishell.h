/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:21 by marvin            #+#    #+#             */
/*   Updated: 2024/06/27 23:46:23 by marvin           ###   ########.fr       */
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
# include <errno.h>
# include <termcap.h>
# include <termios.h>
# include "./mylib/mylib.h"

# define ENV_VAR    __environ
# define PROMPT		"\033[42;34m>> minishell: \033[0;37m"

extern int	g_sig;

typedef struct s_chunk {
	char	**infiles; //nome dos files
	int		nmb_inf; //numero total
	int		*here_dcs; //valores da flag
	int		heredoc; // --- //
	char	*here_file; // --- //
	char	*delimiter;
	char	**outfiles; //
	int		nmb_outf; //
	int		append;
	int		*app_dcs; //
	char	*og;
	char	**cmd_n_args;
	char	*path; // --- //
	int		inpipe;
	int		inpfd; // --- //
	int		outpipe; // --- //
	int		outpfd; // --- //
	int		blt;
}	t_chunk;

/*
heredoc passa a funcionar como uma flag mas na pratica
é o mesmo valor que o heredoc_final de
here_dcs[nmb_inf]

int		*here_dcs;
char	*here_file;
char	**infiles <- char *infile
int		nmb_inf;
int		final_inf;
char	**outfiles <- char *outfile
int		nmb_outf;
int		final_outf;

char *herefile ???? que é isto
*/

typedef struct s_execlist {
	t_chunk	**chunk;
	int		cmd_nmb;
	int		pipe_nmb;
	char	***my_envp;
	int		valid_cmds;
	int		*pipe_loc;
	int		*exit_stt;
	int		*to_end;
	int		*env_pipe;
}	t_execlist;
/*
-> valid_cmds && cmd_nmb vai dar ao mesmo
-> tirar inpfd, outpfd e outpipe
-> tmb posso tirar o path, nao o vou usar p nada
*/

// PRINTS
void	print_db_char(char **str);
void	print_db(char **str);
void	support_print(t_execlist *execl, int c);

// SIGNAL HANDLING
void	sig_handlerr(int mode);
//void	sig_repeat(int num);

// PARSER MAIN
int		parse_central(t_execlist **execl, char *input, int *exit_stt, \
		char ***env);
int		pipe_chunks(t_execlist **execl, char *input, int *ex_stt, \
		char ***env);
int		redir_checker(t_execlist *execl);
void	updt_rdr_lst(t_chunk *chunk, int in_out, int flag, char *n_str);
int		scope_redirs(t_execlist *execl);
int		special_char(t_execlist *execl);
int		arg_separator(t_execlist *execl);
int		arg_id(t_execlist *execl);

// PARSER SUPPORT
int		pipe_counter(char *input, t_execlist *execl);
int		input_redir(t_chunk *chunk, int *i, char *nwe);
int		output_redir(t_chunk *chunk, int *i, char *nwe);
char    *heredoc_read(char *lim);
char	*new_chnk(char *spec, char *old, int a, int b);
int		cmd_separator(t_chunk *chunk);
void	add_arg(t_chunk *chunk, char **str);
int		chunk_id(t_chunk *chunk, int opt);
char	*search_my_envp(t_execlist *execl, char *env_name);
void	updt_rdr_lst(t_chunk *chunk, int in_out, int flag, char *n_str);

// GLOBAL SUPPORT
void	ft_exit(int *err, char **cmd, t_execlist *execl);
char	**create_envp(void);
void	close_pipes(t_execlist *execl, int **fd, int i, int rel, int non_rel);
char	*rmv_newline(char *old);

// EXECUTOR
int		exec_main(t_execlist *execl);
void	exec_loop(t_execlist *execl, int **fd, char ***exec_str);
void	exec_input(t_execlist *execl, int **fd, int i);
void	exec_output(t_execlist *execl, int **fd, int i);
void	ex_outfile(t_execlist *execl, int **fd, int i);
void	temp_pipe(int *nfd, char *buff);
char	*empty_pipe(int fd, t_execlist *execl, int i);
char	***read_from_pipe(int fd, t_execlist *execl);
void	write_to_pipe(int fd, char ***envs);
void	open_all_redirs(t_execlist *execl);

// FREE
char	*free_str(char *str);
int		*free_int_arr(int *str);
char	**free_db_str(char **str);
t_chunk		*free_chunk(t_chunk *chunk);
t_execlist	*free_exec(t_execlist *execl);

// BUILT-IN
void	blt_central(t_execlist *execl, int i, char **exec_str, int mode);
void	ft_cd(int *err, char **cmd, char ***env);
void	ft_echo(int *err, char **cmd);
void	ft_env(int *err, char **cmd, char ***envp);
//void	ft_exit(int *err, char **cmd, t_execlist *execl);
void	ft_export(int *err, char **cmd, char ***envp);
void    ft_pwd(int *err, char **cmd, char **envp);
void	ft_unset(int *err, char **cmd, char ***envp);

#endif