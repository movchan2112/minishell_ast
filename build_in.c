#include "minishell.h"

int ft_echo(t_cmd *cmd)
{
	int n = 1;
	int i = 0;
	if(cmd->args[1])
		if(ft_strcmp(cmd->args[1],"-n") == 0)
		{
			i = 1;
			n++;
		}
	while (cmd->args[n])
	{
		ft_putstr(cmd->args[n]);
		if (cmd->args[n + 1])
			ft_putstr(" ");
		n++;
	}
	if(i == 0)
		ft_putstr("\n");
	// g_exit_status = 0;
	return 0;
}

void	ft_env(t_shell *shell)
{
	while (shell->env)
	{
		if (shell->env->val)
			printf("%s=%s\n", shell->env->key, shell->env->val);
			shell->env = shell->env->next;
	}
}

/// FOR CD

