/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:21 by marvin            #+#    #+#             */
/*   Updated: 2024/05/22 06:04:06 by marvin           ###   ########.fr       */
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
# include <termcap.h>
# include <termios.h>
# include "./mylib/mylib.h"

# define ENV_VAR    __environ
# define PROMPT		"\033[42;34m>> minishell: \033[0;37m"

typedef struct s_chunk {
	char	**infiles; //
	int		nmb_inf; //
	int		heredoc;
	char	*delimiter;
	char	**outfiles; //
	int		nmb_outf; //
	int		append;
	char	*og;
	char	**cmd_n_args;
	char	*path;
	int		inpipe;
	int		inpfd;
	int		outpipe;
	int		outpfd;
	int		blt;
}	t_chunk;

/*
char	**infiles;
int		nmb_inf;
int		final_inf;
char	**outfiles;
int		nmb_outf;
int		final_outf;
*/

typedef struct s_execlist {
	t_chunk	**chunk;
	int		cmd_nmb;
	int		pipe_nmb;
	char	***my_envp;
	int		valid_cmds;
	int		*pipe_loc;
	int		*exit_stt;
	int		*env_pipe;
}	t_execlist;
/*
-> valid_cmds && cmd_nmb vai dar ao mesmo
-> tirar inpfd, outpfd e outpipe
*/

void	print_db_char(char **str);

// SIGNAL HANDLING
void	sig_handler(int mode);
void	sig_repeat(int num);

// PARSER MAIN
int		parse_central(t_execlist **execl, char *input, int *exit_stt, \
		char ***env);
int		pipe_chunks(t_execlist **execl, char *input, int *ex_stt, char ***env);
int		redir_checker(t_execlist *execl);
int		scope_redirs(t_execlist *execl);
int		special_char(t_execlist *execl);
int		arg_separator(t_execlist *execl);
int		arg_id(t_execlist *execl);

// PARSER SUPPORT
int		pipe_counter(char *input, t_execlist *execl);
int		input_redir(t_chunk *chunk, int *i);
int		output_redir(t_chunk *chunk, int *i);
char	*new_chnk(char *spec, char *old, int a, int b);
int		cmd_separator(t_chunk *chunk);
void	add_arg(t_chunk *chunk, char **str);
int		chunk_id(t_chunk *chunk, int opt);
char	*search_my_envp(t_execlist *execl, char *env_name);

// GLOBAL SUPPORT
char	**create_envp(void);
void	close_pipes(t_execlist *execl, int **fd, int i, int rel, int non_rel);
char	*rmv_newline(char *old);

// EXECUTOR
int		exec_main(t_execlist *execl);
void	exec_loop(t_execlist *execl, int **fd, char ***exec_str);
void	exec_input(t_execlist *execl, int **fd, int i);
void	exec_output(t_execlist *execl, int **fd, int i, char ***exec_str);
void	ex_outfile(t_execlist *execl, int **fd, int i, char ***exec_str);
void	temp_pipe(int *nfd, char *buff);
char	*empty_pipe(int fd, t_execlist *execl, int i);
char	***read_from_pipe(int fd, t_execlist *execl);
void	write_to_pipe(int fd, char ***envs);
//void	open_all_redirs(t_execlist *execl, int i);

// FREE
char	*free_str(char *str);
char	**free_db_str(char **str);
void	free_chunk(t_chunk *chunk);
void	free_exec(t_execlist *exec);

// BUILT-IN
void	blt_central(t_execlist *execl, int i, char **exec_str);
void	ft_cd(int *err, char **cmd, char ***env);
void	ft_echo(int *err, char **cmd);
void	ft_env(int *err, char **cmd, char ***envp);
void	ft_exit(int *err, char **cmd);
void	ft_export(int *err, char **cmd, char ***envp);
void    ft_pwd(int *err, char **cmd, char **envp);
void	ft_unset(int *err, char **cmd, char ***envp);

#endif