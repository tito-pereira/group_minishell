/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:34 by marvin            #+#    #+#             */
/*   Updated: 2024/06/25 18:39:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
(2) - reads and stores redirections and filenames inside each token
inicializa cada chunk
*/

int	check_redir(t_execlist *execl, int *i, int c)
{
	char	*nwe;

	nwe = NULL;
	//ft_printf("Inside check_redir;\n"); //
	if (execl->chunk[c]->og[*i] == '<' && c == 0)
	{
		if (input_redir(execl->chunk[c], i, nwe) == -1)
		{
			perror("Input redirection parsing error");
			*(execl->exit_stt) = 1;
			return (0);
		}
	}
	else if (execl->chunk[c]->og[*i] == '>')
	{
		if (output_redir(execl->chunk[c], i, nwe) == -1)
		{
			perror("Output redirection parsing error");
			*(execl->exit_stt) = 1;
			return (0);
		}
	}
	return (1);
}

int	redir_checker(t_execlist *execl)
{
	int	i;
	int	c;

	//ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n"); //
	//ft_printf("Inside parsing (2): redir_checker;\n"); //
	c = -1;
	sig_handlerr(2);
	//ft_printf("Is it here?\n"); //
	//if (execl == NULL) //
		//ft_printf("execl is NULL dumbass\n"); //
	while (execl->chunk[++c] != NULL)
	{
		//ft_printf("Loop [%d];\n", c);
		execl->chunk[c]->infiles = NULL;
		execl->chunk[c]->nmb_inf = -1;
		execl->chunk[c]->here_dcs = NULL;
		execl->chunk[c]->heredoc = 0; //?
		execl->chunk[c]->here_file = NULL; //?
		execl->chunk[c]->delimiter = NULL;
		execl->chunk[c]->outfiles = NULL;
		execl->chunk[c]->nmb_outf = -1;
		execl->chunk[c]->append = 0;
		execl->chunk[c]->app_dcs = NULL;
		execl->chunk[c]->inpipe = 0;
		execl->chunk[c]->path = NULL;
		execl->chunk[c]->cmd_n_args = NULL;
		i = -1;
		while (execl->chunk[c]->og[++i] != '\0')
		{
			if (check_redir(execl, &i, c) == 0)
				return (0);
		}
		//support_print(execl, c);
	}
	return (1);
}

/*
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
*/