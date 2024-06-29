/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:47 by marvin            #+#    #+#             */
/*   Updated: 2024/06/28 02:34:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
(4) - environment variable expander with '$'
	- handles single && double quotes
	- does not handle redirections (handled previously)
*/

/*
nao esquecer que "" suporta $ mas '' já não

Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.

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
		&& chunk[*i] != '\0' && chunk[*i] != 34 && chunk[*i] != '\n')
		(*i)++;
	*b = (*i) - 1;
	ft_printf("a:%d && b:%d\n", *a, *b);
	/*
	se calhar acrescentar uma condicao de [i] != 34 por causa da
	double quote
	ha alguma env var que possua double quote? nao creio
	9, 32, 34, $, \0, e agr \n?
	*/
}

char	*get_spec(int *a, int *b, char *chunk, t_execlist *execl)
{
	char	*env_name;
	char	*env_value;

	ft_printf("Getting spec:\n");
	env_name = ft_substr(chunk, ((*a) + 1), ((*b) - (*a)));
	ft_printf("env_name: '%s'\n", env_name);
	if (!env_name)
		return(NULL);
	if (env_name[0] == '?')
	{
		printf("will go for exit_stt itoa\n");
		env_value = ft_itoa(*(execl->exit_stt));
	}
	else
	{
		printf("will go for search_my_envp\n");
		env_value = search_my_envp(execl, env_name); //getenv(env_name);
	}
	if (!env_value)
		return(NULL);
	free(env_name);
	ft_printf("spec: '%s'\n", env_value);
	return (env_value);
}

int	h_env_var(int *a, int *b, int *i, char **chunk, t_execlist *execl)
{
	char	*spec;

	ft_printf("Inside the handler\n");
	get_positions(a, b, i, *chunk);
	spec = ft_strdup(get_spec(a, b, *chunk, execl));
	if (spec != NULL)
	{
		*chunk = new_chnk(spec, *chunk, *a, *b);
		if (*chunk == NULL)
		{
			ft_printf("New chunk is NULL\n");
			perror("Error handling environment variable ($)");
			return (0);
		}
		ft_printf("New chunk: '%s'\n", *chunk);
	}
	else
	{
		ft_printf("Spec is NULL\n");
		perror("Error handling environment variable ($)");
		return (0);
	}
	return (1);
}

int	spec_char_chunk(t_execlist *execl, int j, int *a, int *b)
{
	int	i;
	int	flag;
	
	i = -1;
	flag = 1;
	while (execl->chunk[j]->og[++i] != '\0')
	{
		if (execl->chunk[j]->og[i] == 39)
			flag *= -1;
		if (execl->chunk[j]->og[i] == '$' && flag == 1)
		{
			if (h_env_var(a, b, &i, &execl->chunk[j]->og, execl) == 0)
			{
				*(execl->exit_stt) = 1;
				return (0);
			}
		}
	}
	return (1);
}

int	spec_char_heredoc(t_execlist *execl, int j, int *a, int *b)
{
	int	i;
	int	flag;
	int	inf;
	
	inf = -1;
	//printf("inside heredoc spec char\n");
	while (execl->chunk[j]->infiles[++inf] != NULL)
	{
		i = -1;
		flag = 1;
		while (execl->chunk[j]->here_dcs[inf] == 1
			&& execl->chunk[j]->infiles[inf][++i] != '\0')
		{
			if (execl->chunk[j]->infiles[inf][i] == 39)
				flag *= -1;
			if (execl->chunk[j]->infiles[inf][i] == '$' && flag == 1)
			{
				printf("special $ sign found in %d position\n", i);
				if (h_env_var(a, b, &i, &execl->chunk[j]->infiles[inf], execl) == 0)
				{
					*(execl->exit_stt) = 1;
					return (0);
				}
			}
		}
	}
	return (1);
}

int	special_char(t_execlist *execl)
{
	int		a;
	int		b;
	int		j;
	//int		i;
	//int		flag;

	j = -1;
	//printf("inside parser 3: spec char\n");
	a = 0;
	b = 0;
	while (execl->chunk[++j] != NULL)
	{
		if (spec_char_chunk(execl, j, &a, &b) == 0)
			return (0);
		if (execl->chunk[j]->infiles)
		{
			//printf("will enter heredoc spec char\n");
			if (spec_char_heredoc(execl, j, &a, &b) == 0)
				return (0);
		}
	}
	return(1);
}
//else, retorna normalmente sem fazer nada

/*
i = -1;
while (execl->chunk[j]->here_dcs[x] == 1
	&& execl->chunk[j]->infiles[x][i] != '\0')
{
	a = 0;
	b = 0;
	if (execl->chunk[j]->infiles[x][i] == 39)
		flag *= -1;
	if (execl->chunk[j]->infiles[x][i] == '$' && flag == 1)
	{
		if (h_env_var(&a, &b, &i, &execl->chunk[j]->infiles[x], execl) == 0)
		{
			*(execl->exit_stt) = 1;
			return(0);
		}
	}
}

execl->chunk->infiles[i] = char * com o heredoc
execl->chunk->here_dcs[i] = int c flag se é heredoc ou nao

ou eu aplico o heredoc ao og original e retiro o <<lim
ou entao tenho que criar na mesma isto aqui

typedef struct s_chunk {
	char	**infiles; //nome dos files
	int		nmb_inf; //numero total
	int		*here_dcs; //valores da flag
	int		heredoc; // --- //
	char	*here_file; // --- //
	char	*delimiter;
	char	**outfiles; //
	int		nmb_outf; //
	int		append;
	int		*app_dcs; //
	char	*og;
	char	**cmd_n_args;
	char	*path; // --- //
	int		inpipe;
	int		inpfd; // --- //
	int		outpipe; // --- //
	int		outpfd; // --- //
	int		blt;
}	t_chunk;
*/

/*
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

/*
int	special_char(t_execlist *execl)
{
	int		a;
	int		b;
	int		j;
	int		i;
	int		flag;

	j = -1;
	flag = 1;
	while (execl->chunk[++j] != NULL)
	{
		i = -1;
		while (execl->chunk[j]->og[++i] != '\0')
		{
			a = 0;
			b = 0;
			if (execl->chunk[j]->og[i] == 39)
				flag *= -1;
			if (execl->chunk[j]->og[i] == '$' && flag == 1)
			{
				if (h_env_var(&a, &b, &i, &execl->chunk[j]->og, execl) == 0)
				{
					*(execl->exit_stt) = 1;
					return(0);
				}
			}
		}
	}
	return(1);
}
*/