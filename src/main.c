/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:44:04 by marvin            #+#    #+#             */
/*   Updated: 2024/07/01 02:33:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sig;

char	*ft_read()
{
	char	*input;

	input = NULL;
	while (1)
	{
		//ft_printf("global_signal check: %d\n", global_sig);
		input = readline(PROMPT);
		if (input != NULL) //normal + ctrl-\ && ctrl-D ignores
		{
			add_history(input);
			return (input);
		}
		else if (input == NULL) //ctrl-D stoppage, SIGINT (ctrl-\ ignores)
			exit(0);
	}
}

char	**create_envp(void)
{
	char	**my_envp;
	int		i;

	i = 0;
	while (__environ[i] != NULL)
		i++;
	my_envp = (char **)malloc((i + 1) * sizeof(char *));
	i = -1;
	while (__environ[++i] != NULL)
		my_envp[i] = ft_strdup(__environ[i]);
	my_envp[i] = NULL;
	return(my_envp);
}

int	null_input(char *input, int *exit_stt)
{
	int	i;

	i = -1;
	if (input[0] == '\0')
	{
		perror("Empty input");
		*exit_stt = 1;
		return (0);
	}
	while (input[++i])
	{
		if (input[i] != ' ' && input[i] != '	')
			return (1);
	}
	perror("Empty input");
	*exit_stt = 1;
	return (0);
}

int	parse_central(t_execlist **execl, char *input, int *exit_stt, char ***env)
{
	int			flag;

	//ft_printf("Inside parsing.\n");//
	flag = null_input(input, exit_stt);
	if (flag == 1)
		flag = pipe_chunks(execl, input, exit_stt, env);
	//printf("p1. exit stt is %d\n", (*(*execl)->exit_stt));
	if (flag == 1)
		flag = redir_checker(*execl);
	//printf("p2. exit stt is %d\n", (*(*execl)->exit_stt));
	if (flag == 1)
		flag = special_char(*execl);
	//printf("p3. exit stt is %d\n", (*(*execl)->exit_stt));
	if (flag == 1)
		flag = scope_redirs(*execl);
	//printf("p4. exit stt is %d\n", (*(*execl)->exit_stt));
	if (flag == 1)
		flag = arg_separator(*execl);
	//printf("p5. exit stt is %d\n", (*(*execl)->exit_stt));
	if (flag == 1)
		flag = arg_id(*execl);
	//printf("p6. exit stt is %d\n", (*(*execl)->exit_stt));
	//ft_printf("Finished parsing with flag %d\n", flag);//
	return (flag);
}

int	main(void)
{
	char			*input;
	t_execlist		*execl;
	int				exit_stt;
	char			**env;

	exit_stt = 0;
	g_sig = 0;
	execl = NULL;
	env = create_envp();
	while (1)
	{
		g_sig = 128;
		sig_handlerr(1);
		input = ft_read();
		/*if (ft_strncmp(input, "exit", 10) == 0)
		{
			ft_printf("Closing minishell...\n");
			exit(0);
		}*/
		if (parse_central(&execl, input, &exit_stt, &env) == 1)
		{
			//printf("main/pre-exec. exit stt is %d\n", (*execl->exit_stt));
			if (exec_main(execl) == 1)
			{
				//printf("main/post-exec. exit stt is %d\n", (*execl->exit_stt));
				env = *(execl->my_envp);
				exit_stt = 0;
			}
		}
		else
		{
			printf("need to handle parser fails\n");
			if (execl)
				free_exec(execl, 1);
			//if (g_sig == 130)
			continue ; //podera haver algum erro que n seja para reset prompt?
			//exit (0);
		}
		printf("returned to main\n");
		if (execl && execl->cmd_nmb == 1
			&& ft_strncmp(execl->chunk[0]->cmd_n_args[0], "exit", 10) == 0)
			ft_exit(execl->chunk[0]->cmd_n_args, execl);
		/*if (execl && execl->cmd_nmb == 1
			&& ft_strncmp(execl->chunk[0]->cmd_n_args[0], "exit", 10) == 0)
		{
			free_exec(execl);
			exit (0); //err_stt
		}*/
		if (execl)
			free_exec(execl, 1);
	}
}

/*
exit
exit | ls
ls | exit
ls | exit | ls | exit
exitt
exit 100
exit 100 23450
exit 4635t
exit 0000000000
exit abc

*/