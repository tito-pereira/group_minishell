#include "../minishell.c"

void	remove_var(char *var, char **envp)
{
	int	i;
	char	*temp_var;

	if (!var || !envp)
		return ;
	temp_var = ft_strjoin(var, "=");
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], temp_var, ft_strlen(temp_var)))
		i++;
	if (envp && envp[i])
	{
		envp[i] = free_ptr(envp[i]);
		envp[i] = envp[i + 1];
		i++;
		while (envp[i])
		{
			envp[i] = envp[i + 1]
			i++;
		}
		envp[i] = NULL;
	}
	temp_var = free_ptr(temp_var);
}

void	ft_unset(t_mini *ms, char **cmd, char **envp)
{
	while (*(++cmd))
		remove_var(*cmd, envp);
	ms->error = 0;
}
