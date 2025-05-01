#include "minishell.h"

int buildin_checker(char *cmd)
{
    if(!cmd)
        return 0;
    char *argc1[] = {"cd","echo","pwd","exit","export","unset","env"};    // check len after in while loop
    int i = 0;
    while (i < 7) // <----------here
    {
        if(ft_strcmp(cmd, argc1[i]) == 0)
            return(1);
        i++;
    }
    return(0);
}
void find_buildin(t_shell *shell)
{
    t_cmd *cmd = shell->ast->cmd;
    if(!ft_strcmp(cmd->cmd, "echo"))
        ft_echo(cmd);
    if(!ft_strcmp(cmd->cmd, "env"))
        ft_env(shell);
    //to-do what left
    return ;
}