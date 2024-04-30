/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:29 by marvin            #+#    #+#             */
/*   Updated: 2024/04/30 12:57:01 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
(1) - separar em chunks divididos por pipes
(cada chunk vai ter um shell command diferente)
*/

int	pipe_counter(char *input)
{
	int	c;
	int	i;

	c = 0;
	i = -1;
	while (input[++i] != '\0')
	{
		if (input[i] == '|')
		{
			if (input[i + 1] == '|')
				return (-1);
			if (input[i + 1] != '|')
				c++;
			if (input[i + 1] == '\0')
				return (-1);
		}
	}
	return (c);
}

/*
if ||, error
nao esta a dar esse error (ou retorno -1)
*/

int	chunk_create(char *input, t_execlist *execl, int *exit_stt)
{
	char	**og_group;
	int		i;

	ft_printf("Inside chunk_create.\n");
	og_group = ft_split(input, '|');
	i = 0;
	while (og_group[i] != NULL)
		i++;
	if (i != execl->cmd_nmb)
	{
		perror("Pipe parsing error");
		//free_exec(execl);
		//exit (0);
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
	return(1);
}

// transformar em retorno int
//t_execlist	*pipe_chunks(char *input, int *exit_stt)
int	pipe_chunks(t_execlist **execl, char *input, int *exit_stt)
{
	int			c;
	//t_execlist	*execl;

	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (1): pipe_chunks;\n");
	if (*execl == NULL)
		ft_printf("execl is NULL dumbass\n");
	*execl = malloc(sizeof(t_execlist));
	if (!(*execl))
	{
		perror("Memory allocation problem in the parser");
		//exit (0);
		*exit_stt = 1;
		return(0);
	}
	c = pipe_counter(input);
	if (c == -1)
	{
		perror("Invalid pipe placement");
		//free_exec(execl);
		//exit (0);
		*exit_stt = 1;
		return(0);
	}
	(*execl)->chunk = malloc ((c + 2) * sizeof(t_chunk *));
	(*execl)->cmd_nmb = c + 1;
	(*execl)->pipe_nmb = c;
	(*execl)->chunk[c + 1] = NULL; //
	ft_printf("Pipe counter: %d;\n", c);
	if (chunk_create(input, *execl, exit_stt) == 0)
		return(0);
	if (*execl)
	{
		ft_printf("execl exists and has %d chunks:\n", (*execl)->cmd_nmb);
		for (int i = 0; (*execl)->chunk[i] != NULL; i++)
			ft_printf("chunk[%d]->og: %s;\n", i, (*execl)->chunk[i]->og);
	}
	//return (execl);
	return(1);
}

/*
- pipe_counter (nao usa execl)
- chunk_create (j usa execl)
*/

/*
- recebe char *input original;
- separa em char * chunks que dividem os vários pipes em chunks de comando
- retorna a main struct execlist já com os varios chunks separados

erros
- presenca de ||
- split / chunk nmb != cmd_nmd

.como nao vou implementar o simbolo ||, nao suporto essa funcionalidade
aqui e uso a ft_split sem ter isso em conta

. colocar erros de pipe placement ou apenas erro de || e o
resto eventualmente faz-se erros de execução de comandos?
porque se ponho erro no [0], tmb tenho que por no [1], [2],
etc, porque é muito cedo para colocar pipes, e tinha que fazer
erros para ver se cada chunk tem apenas 1 ou mais comandos etc..
assim é mais simples e apenas faço error check nos comandos,
menos codigo, um bcd menos info, mas a mesma eficacia
*/