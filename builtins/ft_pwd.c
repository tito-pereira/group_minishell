#include "../minishell.h"

void ft_pwd(t_mini *ms, char **cmd, char **envp)
{
	int	i;

	if (!cmd[1])
	{
		i = 0;
		while (envp && ft_strncmp(envp[i], "PWD=", 4))
			i++;
		if (envp && envp[i])
			ft_printf("%s", envp[i] + 4);
	}
	else
	{

		ft_putstr_fd("Bad usage, burro.", 2);
		ms->error = 69;
	}
}
