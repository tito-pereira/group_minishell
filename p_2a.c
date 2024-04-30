/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:40 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 17:44:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *str, int i)
{
	int	a;
	int	b;

	while (str[i] == 9 || str[i] == 32)
		i++;
	if (str[i] == '\0')
		return (NULL);
	a = i;
	while (str[i] != 9 && str[i] != 32 && str[i] != '\0')
		i++;
	if (str[i] == '\0')
		return (NULL);
	b = i - 1;
	return (ft_substr(str, a, (b - a)));
}

char	*ft_str_find(char *str, char *lim)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] == lim[j])
		{
			j++;
			if (lim[j] == '\0')
			{
				ret = ft_substr(str, 0, i);
				//free (str);
				return (ret);
			}
		}
		i++;
	}
	//free (str);
	return (NULL);
}

char	*heredoc_read(char *lim)
{
	char	*input;

	input = readline("Write the input file's name here: ");
	if (!input)
		return (NULL);
	return (ft_str_find(input, lim));
}

/*
esta ordem nao bate certo
porque haveria eu de saltar à frente da primeira palavra
e só depois começar o parser pelo delimiter?
*/

int	input_redir(t_chunk *chunk, int i)
{
	i++;
	if(chunk->og[i] == '<') // <<
	{
		chunk->heredoc = 1;
		//while ((chunk->og[i] == 9 || chunk->og[i] == 32) && chunk->og[i] != '\0')
			//i++; // whitespaces + endstring
		//while (chunk->og[i] != 9 && chunk->og[i] != 32 && chunk->og[i] != '\0')
			//i++; // non whitespaces + endstring
		while ((chunk->og[i] == 9 || chunk->og[i] == 32) && chunk->og[i] != '\0')
			i++; // whitespaces + endstring
		if (chunk->og[i] == '\0')
			return (0);
		chunk->delimiter = get_name(chunk->og, i);
		chunk->infile = heredoc_read(chunk->delimiter);
		if (chunk->infile == NULL)
			return (-1);
	}
	else if(chunk->og[i] != '<') // <
		chunk->infile = get_name(chunk->og, (i - 1));
	return (1);
}

int	output_redir(t_chunk *chunk, int i)
{
	i++;
	if(chunk->og[i] == '>') // >>
	{
		chunk->append = 1;
		chunk->outfile = get_name(chunk->og, i);
	}
	if(chunk->og[i] != '>') // >
	{
		chunk->append = 0;
		chunk->outfile = get_name(chunk->og, (i - 1));
	}
	return(1);
}

