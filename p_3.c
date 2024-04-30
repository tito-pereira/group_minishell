/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:47 by marvin            #+#    #+#             */
/*   Updated: 2024/04/17 17:05:26 by tibarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
nao esquecer que "" suporta $ mas '' já não

Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.

$PATH, $HOME, $SHELL, $PWD

.ft_substr()
char	*ft_substr(char const *s, unsigned int start, size_t len)

.ft_strcat()
size_t	ft_strlcat(char *dest, const char *src, size_t size)

char *ret = getenv("env_name")

se calhar, por questoes de evitar erros, colocar a = $ && b = last_char
*/

void	get_positions(int *a, int *b, int *i, char *chunk)
{
	ft_printf("Getting positions a && b:\n");
	*a = *i;
	(*i)++;
	while (chunk[*i] != 9 && chunk[*i] != 32 && chunk[*i] != '$'
		&& chunk[*i] != '\0')
		(*i)++;
	*b = (*i) - 1;
	ft_printf("a:%d && b:%d\n", *a, *b);
	/*
	a = i;
	while (execl.chunk[j].og[i] != 9 && execl.chunk[j].og[i] != 32
		&& execl.chunk[j].og[i] != '$' && execl.chunk[j].og[i] != '\0')
		i++;
	b = i - 1;
	*/
}

char	*get_spec(int *a, int *b, char *chunk)
{
	char	*env_name;
	char	*env_value;

	ft_printf("Getting spec:\n");
	env_name = ft_substr(chunk, ((*a) + 1), ((*b) - (*a)));
	ft_printf("env_name: '%s'\n", env_name);
	if (!env_name)
		return(NULL);
	env_value = getenv(env_name);
	if (!env_value)
		return(NULL);
	free(env_name);
	ft_printf("spec: '%s'\n", env_value);
	return (env_value);
}

int	handle_env_var(int *a, int *b, int *i, char **chunk, t_execlist *execl)
{
	char	*spec;

	ft_printf("Inside the handler\n");
	get_positions(a, b, i, *chunk);
	spec = ft_strdup(get_spec(a, b, *chunk));
	if (spec != NULL)
	{
		*chunk = new_chnk(spec, *chunk, *a, *b);
		if (*chunk == NULL)
		{
			ft_printf("New chunk is NULL\n");
			perror("Error handling environment variable ($)");
			free_exec(execl);
			//exit(0);
			return(0);
		}
		ft_printf("New chunk: '%s'\n", *chunk);
	}
	else
	{
		ft_printf("Spec is NULL\n");
		perror("Error handling environment variable ($)");
		free_exec(execl);
		//exit(0);
		return(0);
	}
	return(1);
	/*
	get_positions(&a, &b, &i, execl.chunk[j].og);
	spec = get_spec(a, b, execl.chunk[j].og);
	if (spec != NULL)
		execl.chunk[j].og = new_chnk(spec, execl.chunk[j].og, a, b);
	else
	{
		perror("Error handling environment variable");
		free_exec(execl);
		exit(0);
	}
	*/
}

int	special_char(t_execlist *execl, int *exit_stt)
{
	int		a;
	int		b;
	int		j;
	int		i;
	int		flag;

	j = -1;
	flag = 1;
	ft_printf("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n");
	ft_printf("Inside parsing (3): special_char;\n");
	(void)exit_stt;
	while (execl->chunk[++j] != NULL)
	{
		ft_printf("Chunk n°%d:\n", j);
		i = -1;
		while (execl->chunk[j]->og[++i] != '\0')
		{
			a = 0;
			b = 0;
			if (execl->chunk[j]->og[i] == 39)
				flag *= -1;
			if (execl->chunk[j]->og[i] == '$' && flag == 1)
			{
				ft_printf("Special char found in position %d\n", i);
				if (handle_env_var(&a, &b, &i, &execl->chunk[j]->og, execl) == 0)
					return(0);
				else
					ft_printf("execl->chunk[j]->og: '%s'\n", execl->chunk[j]->og);
			}
		}
	}
	return(1);
}
//else, retorna normalmente sem fazer nada

/*
acho que ja testei tudo, parece estar bom

ja testei com palavras antes, depois, inicio, fim, sozinho
ja testei com pipes
ja testei com $ errados

.fazer e testar o $? que devera expandir para o exit status do ultimo
comando
por default, acho que e zero
default - 0;
success - 0;
errors:
Exit status 1: Generic error code indicating unspecified error.
Exit status 2: Misuse of shell builtins (e.g., incorrect usage of a command).
Exit status 126: Permission problem or command is not executable.
Exit status 127: Command not found or executable cannot be invoked.
Exit status 128+n: Fatal error signal n (where n is a signal number).
Exit status 130: Command terminated by Ctrl+C (SIGINT).
Exit status 255: Exit status out of range or undefined.
parsing + execution both create error status

.first '$HOME' second
(acho que o proprio ponto 4 do parser nao foi testado ent n sei
de onde vem o erro)
*/