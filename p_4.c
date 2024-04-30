	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:55 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 17:44:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
(4) - separar em cmd_n_arg incluindo "" e '', ft_substr c posições

(repeating) skip whitespace -> parse;

. erros para '\0' estao contidos nas funcoes e retornam -1
. retorna 0 sem alterações ou retorna 1 c alterações
. correm seguidos porque só um vai retornar 1 c alterações
erros para unclosed quotes aqui ou noutro sitio

34 "
39 '

. talvez um erro em que cmp_sep e arg_c nao coincidem ?
*/


int	arg_separator(t_execlist *execl)// int *exit_stt)
{
	int		c;
	int		ret;

	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (4): arg_separator;\n");
	(void)execl->exit_stt;
	c = -1;
	while (execl->chunk[++c] != NULL)
	{
		ft_printf("Loop nª%d;\n", c);
		ret = cmd_separator(execl->chunk[c]);
		ft_printf("cmd_separator return == %d;\n", ret);
		if (ret == 0)
		{
			perror("Empty pipe error");
			execl->exit_stt = 1;
			//free_exec(execl);
			//exit(0);
			return(0);
		}
		else if (ret == -1)
		{
			//perror("Unclosed quotes error");
			execl->exit_stt = 1;
			//free_exec(execl);
			//exit(0);
			return(0);
		}
	}
	return(1);
	/*ft_printf("Manual print:\n");
	ft_printf("str[%d]: %s;\n", 0, execl->chunk[0]->cmd_n_args[0]);
	ft_printf("str[%d]: %s;\n", 1, execl->chunk[0]->cmd_n_args[1]);
	ft_printf("str[%d]: %s;\n", 2, execl->chunk[0]->cmd_n_args[2]);*/
}


/*
verificação de vários erros
- vários comandos (mais do que um built in)
- nao contar com as redirections e saber retira-las
*/

/*
-------------------------------

support function for my command separator
instead of creating a command counter and a command parser
in separate, and iterating over the entire input string twice,
i created a dynamic resizer for the char **cmd_n_args
- new malloc +1 size,
- copies old to new,
- adds new substring to last position,
- free old cmd_n_args

adicionar NULL no fim
algo na numeração da atribuição do NULL nao esta a bater bem
pq todos os elementos estao NULL agora
o new[] imprime bem. o problema é com os frees.
algo esta a tranformar os pointer em NULL ou a trocar a ordem deles

atribuo os pointers diretamente new = pointer, em vez de duplicar
burro do crl
eu igualo os pointers mas dou free ao conteudo local na mesma
*/

void	add_arg(t_chunk *chunk, char *str)
{
	int		c;
	char	**new;

	c = 0;
	ft_printf("Adding new arg to cmd_n_args;\nThis bad boy: '%s'\n", str);
	if (chunk->cmd_n_args == NULL)
	{
		ft_printf("First arg;\n");
		chunk->cmd_n_args = malloc(2 * sizeof(char *));
		chunk->cmd_n_args[0] = str;
		chunk->cmd_n_args[1] = NULL;
	}
	else
	{
		ft_printf("Already existing args;\n");
		while (chunk->cmd_n_args[c] != NULL)
			c++;
		ft_printf("How many cmd_n_args then? %d;\n", c);
		new = malloc((c + 2) * sizeof(char *));
		c = -1;
		while (chunk->cmd_n_args[++c] != NULL)
			new[c] = ft_strdup(chunk->cmd_n_args[c]);
		new[c] = str;
		new[c + 1] = NULL;
		ft_printf("Manual print:\n");
		ft_printf("new[%d]: %s;\n", 0, new[0]);
		if (new[1])
			ft_printf("new[%d]: %s;\n", 1, new[1]);
		if (new[2])
			ft_printf("new[%d]: %s;\n", 2, new[2]);
		//ft_printf("Everything good up until here;\n");
		free_db(chunk->cmd_n_args);
		chunk->cmd_n_args = new;
	}
	ft_printf("Successfull addition;\n");
	//ft_printf("Manual print:\n");
	//ft_printf("str[%d]: %s;\n", 0, chunk->cmd_n_args[0]);
}

/*
void	add_arg(t_chunk *chunk, char *str)
{
	int		c;
	char	**new;

	c = 0;
	if (chunk->cmd_n_args == NULL)
	{
		chunk->cmd_n_args = malloc(2 * sizeof(char *));
		chunk->cmd_n_args[0] = str;
		chunk->cmd_n_args[1] = NULL;
	}
	else
	{
		while (chunk->cmd_n_args[c] != NULL)
			c++;
		new = malloc((c + 2) * sizeof(char *));
		c = -1;
		while (chunk->cmd_n_args[++c] != NULL)
			new[c] = chunk->cmd_n_args[c];
		new[c] = str;
		new[c + 1] = NULL;
		free_db(chunk->cmd_n_args);
		chunk->cmd_n_args = new;
	}
}*/


/**
{
	c = 0;
	while (chunk->cmd_n_args[c] != NULL)
		c++;
	new = malloc((c + 2) * sizeof(char *));
	c = -1;
	while (chunk->cmd_n_args[++c] != NULL)
		new[c] = chunk->cmd_n_args[c];
	new[c] = str;
	new[c + 1] = NULL;
	free_db(chunk->cmd_n_args);
	chunk->cmd_n_args = new;
}

c = 0; arg[0] == wassup; c = 1
c = 1; arg[1] == NULL; exit
new = malloc 1 + 2 char * = 3 char *

c = -1;
c++; arg[0] == wassup == new[0];
c++; arg[1] == NULL, exit
new

old
void	arg_separator(t_execlist *execl)
{
	int		i;
	int		c;
	//int		ag;
	int		ret;

	c = -1;
	while (execl->chunk[++c] != NULL)
	{
		i = -1;
		//ag = arg_counter(execl->chunk[c]->og);
		//if (ag == -1)
			return (NULL); //condição de erro
		//execl->chunk[c]->cmd_n_args = malloc (ag * sizeof(char *));
		ret = cmd_separator(execl->chunk[c]);
		if (ret != 1)
			arg_sep_errors(execl, ret);
	}
}

void	arg_sep_errors(t_execlist *execl, int ret)
{
	if (ret == 0)
	{
		perror("Empty pipe error");
		free_exec(execl);
		exit(0);
	}
	else if (ret == -1)
	{
		//perror("Unclosed quotes error");
		free_exec(execl);
		exit(0);
	}
}
*/