#include "../minishell.h"

int	is_only_zeros(char	*nbr)
{
	int	i;

	i = -1;
	while (nbr[++i])
	{
		if (nbr[i] != '0')
			return (0);
	}
	return (1);
}

int	is_valid(char *nbr)
{
	int	i;

	i = 0;
	if (nbr[0] == '+')
		i++;
	while (nbr[i])
	{
		if (!ft_isdigit(nbr[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_mini *ms, char **cmd)
{
	int	code;

	if (!cmd[1])
	{
		free_mat(cmd);
		exit_handler(ms, NULL, 0);
	}
	if (is_only_zeros(cmd[1]))
	{
		free_mat(cmd);
		exit_handler(ms, NULL, 0);
	}
	if (!cmd[2] && is_valid(cmd[1]))
	{
		code = ft_atoi[cmd[1]];
		free_mat(cmd);
		exit_handler(ms, "exited with code: ", code);
	}
	cmd = free_mat(cmd);
	exit_handler(ms,"-" PROMPT_MSG ": exit: too many arguments", 2);
}
