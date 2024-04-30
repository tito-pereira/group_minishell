#include "../minishell.h"

void	ft_env(t_mini *ms, char **cmd, char **envp)
{
	int	i;

	if (!cmd[1])
	{
		i = 0
		while (envp && *envp && envp[i][0])
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
		ms->error = 0;
	}
	else
	{
		ft_putstr_fd("Bad usage, burro." 2);
		ms->error = 69;
	}
}
