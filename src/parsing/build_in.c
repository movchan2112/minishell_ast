#include "minishell.h"

int	remove_env_node(t_env **head, t_env **curr,
					t_env **prev, const char *key)
{
	while (*curr != NULL)
	{
		if (ft_strcmp((*curr)->key, key) == 0)
		{
			if (*prev == NULL)
				*head = (*curr)->next;
			else
				(*prev)->next = (*curr)->next;
			free((*curr)->key);
			free((*curr)->val);
			free(*curr);
			return (0);
		}
		*prev = *curr;
		*curr = (*curr)->next;
	}
	return (0);
}

int	remove_env_variable(t_env **env_list_ptr, const char *key_to_unset)
{
	t_env	*current;
	t_env	*prev;

	if (!is_valid_env_key(key_to_unset))
	{
		fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
			key_to_unset);
		return (1);
	}
	current = *env_list_ptr;
	prev = NULL;
	return (remove_env_node(env_list_ptr, &current, &prev, key_to_unset));
}


int ft_unset(t_shell *shell, t_cmd *cmd)
{
	int i;
	int overall_status;

	i = 1; // Start from cmd->args[1]
	overall_status = 0; // Assume success

	// If no arguments are given, unset does nothing and succeeds
	if (cmd->args[1] == NULL)
	{
		return (0);
	}

	// Process each argument
	while (cmd->args[i] != NULL)
	{
		// remove_env_variable will print error for invalid keys
		// and update overall_status if an error occurs.
		if (remove_env_variable(&(shell->env), cmd->args[i]) != 0)
		{
			overall_status = 1; // Mark that at least one error occurred
		}
		i++;
	}
	return (overall_status);
}

char	*get_oldpwd(void)
{
	return (getcwd(NULL, 0));
}
int	check_cd_args(t_cmd *cmd, char **oldpwd, t_shell *shell)
{
	if (cmd->args[1] && cmd->args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		free(*oldpwd);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}