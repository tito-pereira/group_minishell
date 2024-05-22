/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:40 by marvin            #+#    #+#             */
/*   Updated: 2024/05/22 06:12:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	input_redir(t_chunk *chunk, int *i)
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
		//ft_printf("delimiter is '%s'\n", chunk->delimiter); //
		if (chunk->infile != NULL)
			free(chunk->infile);
		chunk->infile = heredoc_read(chunk->delimiter);
		if (chunk->infile == NULL)
			return (-1);
	}
	else if (chunk->og[*i] != '<') // <
	{
		if (chunk->infile != NULL)
			free(chunk->infile);
		//ft_printf("simple input redirection checking.\n"); //
		chunk->infile = get_name(chunk->og, *i);
	}
	if (chunk->infile == NULL)
		return (-1);
	//ft_printf("heredoc: '%d'\n", chunk->heredoc);//
	//ft_printf("infile: '%s'\n", chunk->infile); //
	return (1);
}

int	output_redir(t_chunk *chunk, int *i)
{
	(*i)++;
	//ft_printf("Output redirection checker.\n"); //
	if(chunk->og[*i] == '>') // >>
	{
		(*i)++;
		//ft_printf("append output redirection.\n");//
		chunk->append = 1;
		if (chunk->outfile != NULL)
			free(chunk->outfile);
		chunk->outfile = get_name(chunk->og, *i);
		(*i)--;
	}
	if(chunk->og[*i] != '>') // >
	{
		//ft_printf("truncate output redirection.\n");//
		chunk->append = 0;
		if (chunk->outfile != NULL)
			free(chunk->outfile);
		chunk->outfile = get_name(chunk->og, *i);
	}
	if (chunk->outfile == NULL)
		return (-1);
	//ft_printf("append: '%d'\n", chunk->append);//
	//ft_printf("outfile: '%s'\n", chunk->outfile);//
	return(1);
}

/*
por exemplo
chunk->outfile[number] = get_name;

em que so chunk->outfile[nmb_outf] seria o valido, porque é o
ultimo
comeca sempre em zero
e if (0), fazer entao um malloc **
*/

char	**add_char_p(char **old, char *son)
{
	char	**new;
	int		i;

	i = 0;
	while (old[i] != NULL)
		i++;
	new = malloc((i + 2) * sizeof(char *));
	i = -1;
	while (old[++i] != NULL)
		new[i] = ft_strdup(old[i]);
	new[++i] = son; //ou strdup ainda n sei
	new[i] = NULL;
	free_db_char(old);
	return (new);
}

void	update_char_p(t_chunk *chunk, char *new_r)
{
	if (chunk->nmb_outf == 0)
	{
		chunk->outfiles = malloc(2 * sizeof(char *));
		chunk->outfiles[0] = son;
		chunk->outfiles[1] = NULL;
	}
	else
	{
		chunk->outfiles = add_char_p(chunk->outfiles, new_r);
	}
}