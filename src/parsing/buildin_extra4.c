#include "minishell.h"


void cleanup_shell(t_shell *shell)
{
    free_token_list(shell->tokens);
    free_ast(shell->ast);
    ft_free_env(shell->env);
}


int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	is_numeric(const char *s)
{
	if (!s || !*s)
		return (0);
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

int	ft_exit(t_shell *shell, t_cmd *cmd)
{
	long	exit_code;

	if (cmd->args[1])
    {
        if (!is_numeric(cmd->args[1]))
        {
            write(2, cmd->args[0], ft_strlen(cmd->args[1]));
            write(2, "\nminishell: exit: numeric argument required\n", 45);
            // cleanup_shell(shell);
            // exit(2);
            return(shell->exit_status = 2);
        }
        if (cmd->args[2])
        {
            write(2, "minishell: exit: too many arguments\n", 36);
            shell->exit_status = 1;
            return (1);
        }
        exit_code = ft_atoi(cmd->args[1]);
        cleanup_shell(shell);
        exit((unsigned char)exit_code);
    }

	cleanup_shell(shell);
	exit(shell->exit_status);
}