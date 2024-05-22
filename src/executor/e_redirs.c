/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:29:03 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/22 18:08:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	open_all_infs(t_execlist *execl, int i)
{
	int	i;
	int	tmp;

	i = -1;
	if (execl->chunk[i]->infiles != NULL)
	{
		while (++i <= execl->chunk[i]->nmb_inf)
		{
			tmp = open();
			if (tmp == -1)
				//;
			close(tmp);
		}
	}
}

void	open_all_outfs(t_execlist *execl, int i)
{
	int	i;
	int	tmp;
	
	i = -1;
	if (execl->chunk[i]->outfiles != NULL)
	{
		while (++i <= execl->chunk[i]->nmb_outf)
		{
			tmp = open();
			if (tmp == -1)
				//;
			close(tmp);
		}
	}
}

void	open_all_redirs(t_execlist *execl, int i)
{
	if (execl->chunk[i]->infiles != NULL)
		open_all_infs(execl, i);
	if (execl->chunk[i]->outfiles != NULL)
		open_all_outfs(execl, i);
}