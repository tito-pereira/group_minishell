#include "minishell.h"

int execbi(t_mini *ms, char **cmd, char ***envp)
{
    if (!ms || !cmd || !envp)
        return (-1);
    if (!ft_strncmp(*cmd, "echo", 5))
        ft_echo(ms, cmd);
    else if (!ft_strncmp(*cmd, "cd", 3))
    else if (!ft_strncmp(*cmd, "pwd", 4))
        
    else if (!ft_strncmp(*cmd, "export", 7))
    else if (!ft_strncmp(*cmd, "unset", 6))
    else if (!ft_strncmp(*cmd, "env", 4))
    else if (!ft_strncmp(*cmd, "exit", 5))
    else
        return (0);
    return (1);
}
