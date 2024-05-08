/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:29 by marvin            #+#    #+#             */
/*   Updated: 2024/05/08 14:03:56 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**divide_pipes(t_execlist *execl, char *input)
{
	char	**og_group;
	int		i;
	int		beg;

	og_group = malloc((execl->valid_pipes + 1) * sizeof(char *));
	i = -1;
	beg = 0;
	if (execl->valid_pipes == 1)
		og_group = ft_strdup();
	else
	{
		while(++i < execl->valid_pipes)
		{
			if ()
			og_group[i] = ft_substr(input, beg, (execl->pipe_loc[i] - beg));
			beg = execl->pipe_loc[i];
		}
	}
	og_group[i] = NULL;
	return (og_group);
}

/*
se conseguir evitar a strdup topzao
*/

int	chunk_create(char *input, t_execlist *execl, int *exit_stt)
{
	char	**og_group;
	int		i;

	ft_printf("Inside chunk_create.\n");
	//og_group = ft_split(input, '|');
	og_group = divide_pipes(execl, input);
	i = 0;
	while (og_group[i] != NULL)
		i++;
	if (i != execl->cmd_nmb)
	{
		perror("Pipe parsing error");
		*exit_stt = 1;
		return(0);
	}
	i = -1;
	while (++i < execl->cmd_nmb)
	{
		execl->chunk[i] = malloc(sizeof(t_chunk));
		execl->chunk[i]->og = og_group[i];
		ft_printf("chunk[%d] created;\n", i);
	}
	free (og_group);
	return(1);
}

int	pipe_chunks(t_execlist **execl, char *input, int *exit_stt)
{
	int			c;

	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (1): pipe_chunks;\n");
	if (*execl == NULL)
		ft_printf("execl is NULL dumbass\n");
	*execl = malloc(sizeof(t_execlist));
	if (!(*execl))
	{
		perror("Memory allocation problem in the parser");
		*exit_stt = 1;
		return(0);
	}
	c = pipe_counter(input, execl);
	if (c == -1)
	{
		perror("Invalid pipe placement");
		*exit_stt = 1;
		return(0);
	}
	(*execl)->chunk = malloc ((c + 2) * sizeof(t_chunk *));
	(*execl)->cmd_nmb = c + 1;
	(*execl)->pipe_nmb = c;
	(*execl)->my_envp = create_envp();
	(*execl)->chunk[c + 1] = NULL;
	ft_printf("Pipe counter: %d;\n", c);
	if (chunk_create(input, *execl, exit_stt) == 0)
		return(0);
	if (*execl)
	{
		ft_printf("execl exists and has %d chunks:\n", (*execl)->cmd_nmb);
		for (int i = 0; (*execl)->chunk[i] != NULL; i++)
			ft_printf("chunk[%d]->og: %s;\n", i, (*execl)->chunk[i]->og);
	}
	return(1);
}
