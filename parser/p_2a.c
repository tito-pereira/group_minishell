/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_2a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:40 by marvin            #+#    #+#             */
/*   Updated: 2024/05/13 15:22:10 by tibarbos         ###   ########.fr       */
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
	// ft_printf("a:%d;\n", a); //
	while (str[i] != 9 && str[i] != 32 && str[i] != '\0'
		&& str[i] != '>' && str[i] != '<')
		i++;
	b = i;
	//ft_printf("b:%d;\n", b); //
	return (ft_substr(str, a, (b - a)));
}

/*
e redirections dentro de aspas??

- e mesmo necessario aquela verificacao do fim do input ser \0?
- deixo estar b = i ou algum caso em que b = i - 1?

hello w
a = char 7 = [6]
b = char 8 = [7]
len 1
portanto sim eu quero o b na char seguinte para garantir uma len correta
b = i sempre
*/

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

/*
void	old_heredoc_chest(char **chest, char **input)
{
	char	*old;
	
	if (!(*chest))
		*chest = ft_strdup(*input);
	else
	{
		old = *chest;
		*chest = ft_strjoin(*chest, "\n");
		free (old);
		old = *chest;
		*chest = ft_strjoin(*chest, *input);
		free (old);
	}
	free(*input);
}*/

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
		chunk->delimiter = get_name(chunk->og, *i);
		//ft_printf("delimiter is '%s'\n", chunk->delimiter); //
		chunk->infile = heredoc_read(chunk->delimiter);
		if (chunk->infile == NULL)
			return (-1);
	}
	else if (chunk->og[*i] != '<') // <
	{
		//ft_printf("simple input redirection checking.\n"); //
		chunk->infile = get_name(chunk->og, *i);
	}
	//ft_printf("infile: '%s'\n", chunk->infile); //
	return (1);
}

/*
assim posso manter a heredoc original e apenas mudar o executor
com a flag heredoc
*/

int	output_redir(t_chunk *chunk, int *i)
{
	(*i)++;
	//ft_printf("Output redirection checker.\n"); //
	if(chunk->og[*i] == '>') // >>
	{
		//ft_printf("append output redirection.\n");//
		chunk->append = 1;
		chunk->outfile = get_name(chunk->og, *i);
	}
	if(chunk->og[*i] != '>') // >
	{
		//ft_printf("truncate output redirection.\n");//
		chunk->append = 0;
		chunk->outfile = get_name(chunk->og, *i);
	}
	//ft_printf("outfile: '%s'\n", chunk->outfile);//
	return(1);
}

/*char	*ft_str_find(char *str, char *lim)
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
				return (ret);
			}
		}
		i++;
	}
	return (NULL);
}*/