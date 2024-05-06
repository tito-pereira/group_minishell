/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_45.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 11:50:43 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/06 11:52:16 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
acrescentar um novo step p_novo, pré ou pos p_4
se calhar pos p_4 da mais jeito
ve os args todos
ve str[0] == '<' || str[0] == '>'
alona novo char ** c tamanho - 2
retira esse arg e o proximo
free na antiga e retorna a nova char *
*/

int		rmv_redirs(t_execlist *execl, int *exit_stt)
{
    ez;
}