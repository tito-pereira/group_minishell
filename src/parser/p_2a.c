/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:40 by marvin            #+#    #+#             */
/*   Updated: 2024/05/22 22:45:01 by marvin           ###   ########.fr       */
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
		&& str[i] != '>' && str[i] != '<')
		i++;
	b = i;
	//ft_printf("b:%d;\n", b); //
	if (a == b)
		return (NULL);
	return (ft_substr(str, a, (b - a)));
}

void	heredoc_chest(char **chest, char **input)
{
	char	*old;
	
	if (!(*chest))
		*chest = ft_strdup(*input);
	else
	{
		old = *chest;
		*chest = ft_strjoin(*chest, *input);
		free (old);
	}
	old = *chest;
	*chest = ft_strjoin(*chest, "\n");
	free(old);
	free(*input);
}

char	*heredoc_read(char *lim)
{
	char	*input;
	char	*chest;

	input = NULL;
	chest = NULL;
	while (1)
	{
		input = readline("heredoc> ");
		if (!input)
			return (NULL);
		if (ft_strncmp(lim, input, 4096) == 0)
		{
			free(input);
			break ;
		}
		heredoc_chest(&chest, &input);
	}
	return (chest);
}

/*
heredoc_read vai retornar mas é o input inteiro e nao um filename
*/

int	input_redir(t_chunk *chunk, int *i, char *new)
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
		new = heredoc_read(chunk->delimiter);
		if (new == NULL)
			return (-1);
		updt_rdr_lst(chunk, in_out, flag, new);
	}
	else if (chunk->og[*i] != '<') // <
	{
		//if (chunk->infile != NULL) -----------
			//free(chunk->infile); ----------
		//chunk->infile = get_name(chunk->og, *i);
		//ft_printf("simple input redirection checking.\n");
		chunk->heredoc = 0;
		new = get_name(chunk->og, *i);
		if (new == NULL)
			return (-1);
		updt_rdr_lst(chunk, in_out, flag, new);
	}
	//if (chunk->infile == NULL) //----------
		//return (-1); //----------
	//ft_printf("heredoc: '%d'\n", chunk->heredoc);
	//ft_printf("infile: '%s'\n", chunk->infile);
	return (1);
}

int	output_redir(t_chunk *chunk, int *i, char *new)
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
		new = get_name(chunk->og, *i);
		if (new == NULL)
			return (-1);
		updt_rdr_lst(chunk, in_out, flag, new);
		(*i)--;
	}
	if(chunk->og[*i] != '>') // >
	{
		//ft_printf("truncate output redirection.\n");//
		chunk->append = 0;
		//if (chunk->outfile != NULL) -----------
			//free(chunk->outfile); ----------
		//chunk->outfile = get_name(chunk->og, *i);
		new = get_name(chunk->og, *i);
		if (new == NULL)
			return (-1);
		updt_rdr_lst(chunk, in_out, flag, new);
	}
	//if (chunk->outfile == NULL) //----------
		//return (-1); //----------
	//ft_printf("append: '%d'\n", chunk->append);//
	//ft_printf("outfile: '%s'\n", chunk->outfile);//
	return(1);
}