/*
c = chunk iteration;
pid, fd, pipe, fork = execve e conexao de pipes

diferentes condicoes pipe flags e redirs para definir os dups
*/

int	exec_chunk(t_execlist *execl)
{
	int	*fd;
	int	pid;
	int	c;

	c = execl.current;
	fd = malloc (2 * sizeof(int));
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (inpipe == 1) // inpipe valido
			dup2(execl.chunk[c].inpfd, STDIN_FILENO);
		else if (execl->chunk[c]->inpipe == 0 && infile != NULL)
			dup2(open(execl.chunk[c].infile, O_RDONLY), STDIN_FILENO);
		if (outfile != NULL)
			dup2(open(execl.chunk[c].outfile, O_RDONLY), STDOUT_FILENO);
		if (outpipe != NULL)
			execl.chunk[c + 1].inpfd = execl.chunk[c].outpfd; //a ser usado no comando seguinte
		execve(execl.chunk[c].cmd_n_args[0], execl.chunk[c].cmd_n_args, execl->my_envp);
	}
	wait(0);
	close(fd[1]);
	return (fd[0]);
}


/*
pega no fd do ultimo pipe fd e copia tudo (getnextline) para o STDOUT
por alguma razao fiz este que apenas printa tudo no stdout???
*/

void	last_output(int last)
{
	char	*ret;
	int		sig;

	sig = 1;
	while (sig == 1)
	{
		ret = get_next_line(last);
		if (ret != NULL)
		{
			ft_printf("%s\n", ret);
			free(ret);
		}
		else
			sig = 0;
	}
	close(last);
}