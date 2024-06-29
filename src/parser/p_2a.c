/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:40 by marvin            #+#    #+#             */
/*   Updated: 2024/06/29 00:52:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_name(char *str, int i)
{
	int	a;
	int	b;

	//ft_printf("Getting file name:\n"); //
	while (str[i] == 9 || str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (NULL);
	a = i;
	//ft_printf("a:%d;\n", a); //
	while (str[i] != 9 && str[i] != 32 && str[i] != '\0'
		&& str[i] != '>' && str[i] != '<' && str[i] != '|')
		i++;
	b = i;
	//ft_printf("b:%d;\n", b); //
	if (a == b)
		return (NULL);
	return (ft_substr(str, a, (b - a)));
}

int	input_redir(t_chunk *chunk, int *i, char *nwe)
{
	(*i)++;
	//ft_printf("Input redirection checker.\n"); //
	if (chunk->og[*i] == '<') // <<
	{
		//ft_printf("heredoc input redirection checking.\n"); //
		chunk->heredoc = 1;
		while ((chunk->og[++(*i)] == 9 || chunk->og[*i] == 32)
			&& chunk->og[*i] != '\0')
			continue ; // whitespaces + endstring
		if (chunk->og[*i] == '\0')
		{
			perror("Empty input redirection");
			return (0);
		}
		if (chunk->delimiter != NULL)
			free(chunk->delimiter);
		chunk->delimiter = get_name(chunk->og, *i);
		if (chunk->delimiter == NULL)
			return (-1);
		//if (chunk->infile != NULL) //----------
			//free(chunk->infile); //----------
		nwe = heredoc_read(chunk->delimiter);
		if (nwe == NULL)
		{
			printf("heredoc_read return was NULL, will give 130\n");
			return (130);
		}
		updt_rdr_lst(chunk, 0, 1, nwe);
	}
	else if (chunk->og[*i] != '<') // <
	{
		//if (chunk->infile != NULL) -----------
			//free(chunk->infile); ----------
		//chunk->infile = get_name(chunk->og, *i);
		//ft_printf("simple input redirection checking.\n");
		chunk->heredoc = 0;
		nwe = get_name(chunk->og, *i);
		if (nwe == NULL)
			return (-1);
		updt_rdr_lst(chunk, 0, 0, nwe);
	}
	//if (chunk->infile == NULL) //----------
		//return (-1); //----------
	//ft_printf("heredoc: '%d'\n", chunk->heredoc);
	//ft_printf("infile: '%s'\n", chunk->infile);
	return (1);
}

int	output_redir(t_chunk *chunk, int *i, char *nwe)
{
	(*i)++;
	//ft_printf("Output redirection checker.\n"); //
	if(chunk->og[*i] == '>') // >>
	{
		(*i)++;
		//ft_printf("append output redirection.\n");//
		chunk->append = 1;
		//if (chunk->outfile != NULL) //----------
			//free(chunk->outfile); //----------
		//chunk->outfile = get_name(chunk->og, *i);
		nwe = get_name(chunk->og, *i);
		if (nwe == NULL)
			return (-1);
		updt_rdr_lst(chunk, 1, 1, nwe);
		(*i)--;
	}
	if (chunk->og[*i] != '>') // >
	{
		//ft_printf("truncate output redirection.\n");//
		chunk->append = 0;
		//if (chunk->outfile != NULL) -----------
			//free(chunk->outfile); ----------
		//chunk->outfile = get_name(chunk->og, *i);
		nwe = get_name(chunk->og, *i);
		if (nwe == NULL)
			return (-1);
		updt_rdr_lst(chunk, 1, 0, nwe);
	}
	//if (chunk->outfile == NULL) //----------
		//return (-1); //----------
	//ft_printf("append: '%d'\n", chunk->append);//
	//ft_printf("outfile: '%s'\n", chunk->outfile);//
	return(1);
}

/*
infile=0, outfile=1, heredoc && append == 1, nao heredoc && trunc == 0
*/

/*
o que raio fazia o nwe neste caso?
o nwe recebe o get_name por isso vai ser either chunk outfile
ou infile, ainda nao sei qual

int		*here_dcs;
char	*here_file;
char	**infiles <- char *infile
int		nmb_inf;
char	**outfiles <- char *outfile
int		nmb_outf;
*/