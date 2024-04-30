#include "../minishell.h"

void	go_to_dir(t_mini *ms, char *dir, char **env)
{
	char	**exp;
	char	*temp;

	exp = ft_calloc(3, sizeof(char *));
	exp[0] = ft_strdup("export");
	if (chdir(dir) == 0)
	{
		temp = getcwd(temp, PATH_MAX);
		exp[1] = ft_strjoin("PWD=", temp);
		ft_export (ms, exp, env);
		ms->error = 0;
	}
	else
	{
		ft_putstr_fd(PROMPT_MSG": cd: No such file or directory", 2);
		ms->error = 69;
	}
	temp = free_ptr(temp);
	exp = free_mat(exp);
}

char	*home_dir(char **env)
{
	char	*home;

	while(env && ft_strncmp(envp[i], "HOME=", 5))
		env++
	home = *env + 5;
	return (home);
}

void	ft_cd(t_mini *ms, char **cmd, char **env)
{
	if (!cmd[1])
		go_to_dir(ms, home_dir(env), env);
	else if (cmd[1] && !cmd[2])
		go_to_dir(ms, cmd[1], env);
	else
	{
		ft_putstr_fd("Erro: Burro.", 2);
		ms->error = 69;
	}
}
