/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:49:36 by tibarbos          #+#    #+#             */
/*   Updated: 2024/05/23 18:02:55 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*empty_pipe(int fd, t_execlist *execl, int i)
{
	char	*shovel;
	char	*chest;
	char	*old;

	if (execl->chunk[i]->infiles == NULL && execl->chunk[i]->inpipe == 1)
		return (NULL);
	printf("inside empty_pipe [%d] with fd[%d]\n", i, fd);
	shovel = get_next_line(fd);
	chest = NULL;
	while (shovel != NULL)
	{
		printf("shovel exists [%d]\n", i);
		if (!chest)
			chest = ft_strdup(shovel);
		else
		{
			old = chest;
			chest = ft_strjoin(chest, shovel);
			free(shovel);
			shovel = NULL;
			free(old);
		}
		shovel = get_next_line(fd);
	}
	ft_printf("final_chest [%d]:%s;\n", i, chest);
	close(fd);
	return (chest);
}

void	temp_pipe(int *nfd, char *buff)
{
	int	pid;
	
	pipe(nfd);
	pid = fork();
	if (pid == 0)
	{
		close(nfd[0]);
		//ft_printf("buff:%s;", buff);
		write(nfd[1], buff, ft_strlen(buff));
		close(nfd[1]);
		exit(0);
	}
	close(nfd[1]);
	wait(NULL);
	dup2(nfd[0], STDIN_FILENO);
	close(nfd[0]);
}
