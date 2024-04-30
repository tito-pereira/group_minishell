/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_5a.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:40:54 by tibarbos          #+#    #+#             */
/*   Updated: 2024/04/17 14:42:55 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rmv_newline(char *old)
{
	char				*new;
	unsigned int		len;

	len = ft_strlen(old);
	ft_printf("Checking for newline.\nOld path size: %d;\n", len);
	if (old[len - 1] == '\n')
	{
		ft_printf("Newline detected, preparing to remove.\n");
		new = malloc(len * sizeof(char));
		ft_strlcpy(new, old, len);
		ft_printf("New non-newline path: '%s'\n", new);
		free(old);
		return(new);
	}
	return(old);
}
// size_t	ft_strlcpy(char *s1, const char *s2, size_t size)
// size ja inclui o '\0'
// strlen[size] == '\0', [size - 1] == '\0'