#include "minishell.h"

int buildin_checker(char *cmd)
{
    if(!cmd)
        return 0;
    char *argc1[] = {"cd","echo","pwd","exit","export","unset","env","$?"};    // check len after in while loop
    int i = 0;
    while (i < 8) // <----------here
    {
        if(ft_strcmp(cmd, argc1[i]) == 0)
            return(1);
        i++;
    }
    return(0);
}
void find_buildin(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
	{
		fprintf(stderr, "minishell: invalid command in find_buildin()\n");
		shell->exit_status = 1;
		return;
	}

	if (!ft_strcmp(cmd->cmd, "echo"))
    {
        shell->exit_status = ft_echo(shell,cmd);
        exit(shell->exit_status);
    }
    else if (!ft_strcmp(cmd->cmd, "env"))
    {
        shell->exit_status = ft_env(shell);
        exit(shell->exit_status);
    }
    else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd();
    else if (!ft_strcmp(cmd->cmd, "export"))
    {
        shell->exit_status = ft_export(shell,cmd);;
        exit(shell->exit_status);
    }
    else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(shell);
    else if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(shell, cmd);
    else if (!ft_strcmp(cmd->cmd, "cd"))
	    shell->exit_status = ft_cd(shell, cmd);
    else if (!ft_strcmp(cmd->cmd, "$?"))
		printf("%d\n",shell->exit_status);
	// TODO: другие команды
}

