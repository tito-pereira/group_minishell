/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:12:29 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/09 17:35:37 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
(2) - remove redirections from the parsing string to simplify proccess

e se houver >>>>> ou <<<<<
redir interpreta isso ou da erro?
-> get_name retorna NULL
-> rmv_redirs salta todos os '<' a frente, assume bom senso
*/

void	temp_strings(char *og, char **new, int a, int b)
{
	char	*first;
	char	*secnd;
	
	first = NULL;
	if (a != 0)
		first = ft_substr(og, 0, a);
	secnd = NULL;
	if (b != ft_strlen(og));
		secnd = ft_substr(og, (b + 1), (ft_strlen(og) - b));
	*new = NULL;
	if (!first && secnd)
		*new = secnd; //(malloc ja feito)
	else if (first && secnd)
	{
		*new = ft_strjoin(first, secnd);
		free(first);
		free(secnd);
	}
	else if (first && !secnd)
		*new = first; //(malloc ja feito)
}

void	find_redirs(char *og, int *a, int *b, int *i)
{
	if (og[*i] == '<' || og[*i] == '>');
	{
		a = *i;
		while (og[*i] == '<' || og[*i] == '>')
			(*i)++;
		while ((og[*i] == 9 || og[*i] == 32) && og[*i] != '\0') //whitespaces + EOF
			(*i)++;
		while (og[*i] != 9 && og[*i] != 32 && og[*i] != '\0') // non white + EOF
			(*i)++;
		b = (*i);
	}
}

/*
se houver <<hbkh>>njne<unoew>kcme vai tudo atrelado ate EOF ou espacos
*/

void	find_red_pos(t_chunk *chunk, int *i)
{
	int		a;
	int		b;
	char	*new;

	a = 0;
	b = 0;
	new = NULL;
	if (chunk->og[*i] == '<' || chunk->og[*i] == '>')
	{
		find_redirs(chunk->og, &a, &b, &i);
		temp_strings(chunk->og, &new, a, b);
		free(chunk->og);
		chunk->og = new;
		(*i) = a;
	}
}

int	scope_redirs(t_execlist *execl, int *exit_stt)
{
	int		c;
	int		i;

	c = -1;
	i = -1;
	(void)exit_stt;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (3): scope_redirs;\n");
	while (execl->chunk[++c] != NULL)
	{
		i = -1;
		while (execl->chunk[c]->og[++i] != '\0')
			find_red_pos(execl->chunk[c], &i);
	}
	return (1);
}

/*
algum error status aqui?

< text
<\0 (erro lidado previamente)

<>text\0 || <> text\0
text<>text\0
text<>\0

ajustar linhas main
testar b pointer (b == index EOF)
i = a final (a = index)
*/