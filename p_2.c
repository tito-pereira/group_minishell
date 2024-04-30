/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:34 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 17:03:45 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
(2) - ler e anotar redirections e file names em cada chunk
*/

int	check_redir(t_execlist *execl, int i, int c, int *exit_stt)
{
	//ft_printf("Inside check_redir;\n");
	if(execl->chunk[c]->og[i] == '<' && c == 0)
	{
		if (input_redir(execl->chunk[c], i) == -1)
		{
			perror("Input redirection parsing error");
			*exit_stt = 1;
			return(0);
		}
	}
	else if(execl->chunk[c]->og[i] == '>')
	{
		if (output_redir(execl->chunk[c], i) == 1)
		{
			perror("Output redirection parsing error");
			*exit_stt = 1;
			return(0);
		}
	}
	return(1);
}

int	redir_checker(t_execlist *execl, int *exit_stt)
{
	int	i;
	int	c;

	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (2): redir_checker;\n");
	//(void)execl->exit_stt;
	c = -1;
	ft_printf("Is it here?\n");
	if (execl == NULL)
		ft_printf("execl is NULL dumbass\n");
	while (execl->chunk[++c] != NULL)
	{
		ft_printf("Loop [%d];\n", c);
		execl->chunk[c]->infile = NULL;
		ft_printf("infile: %s;\n", execl->chunk[c]->infile);
		execl->chunk[c]->heredoc = 0;
		ft_printf("heredoc: %d;\n", execl->chunk[c]->heredoc);
		execl->chunk[c]->delimiter = NULL;
		ft_printf("delimiter: %s;\n", execl->chunk[c]->delimiter);
		execl->chunk[c]->outfile = NULL;
		ft_printf("outfile: %s;\n", execl->chunk[c]->outfile);
		execl->chunk[c]->append = 0;
		ft_printf("append: %d;\n", execl->chunk[c]->append);
		execl->chunk[c]->cmd_n_args = NULL;
		ft_printf("cmd_n_args: %s;\n", execl->chunk[c]->cmd_n_args);
		i = -1;
		while(execl->chunk[c]->og[++i] != '\0')
		{
			//ft_printf("Inside while;\n");
			if (check_redir(execl, i, c, exit_stt) == 0)
				return(0);
		}
	}
	return(1);
}

/*
- extra support functions (linhas ultrapassam limite)
- extra support files (numero funcoes ultrapassa limite)
- heredoc + delimiter (Y)
- if inside pipe, infiles do not work (Y)

// (1) parse_execl()
typedef struct s_chunk {
	char	*infile; // (2) redir_checker
	int		heredoc; // (2) redir_checker
	char	*delimiter; // (2) redir_checker
	char	*outfile; // (2) redir_checker
	int		append; // (2) redir_checker
	char	*og; // (1) parse_execl
	char	**cmd_n_args;
	int		inpipe;
	int		inpfd;
	int		outpipe; //acho que já não é necessário
	int		outpfd;
}	t_chunk;

if (get_name() == NULL)
{
	perror("Incorrect redirection file name");
	free_execl(execl);
	exit(0);
}


< - input, get output from already written file name
<< - input, followed by a DELIMITER and will await user input
> - output, substitute everything on output file
>> - output, add everything to output file

multiple redirections, apenas a ultima conta
files intermedios nem sequer sao criados
*/