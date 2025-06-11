#include "minishell.h"

void print_export(t_env *env1)
{
	t_env *env = env1;
	while (env)
	{
		printf("%s", env->key);
		if (env->val)
			printf("=\"%s\"", env->val);
		printf("\n");
		env = env->next;
	}
}

int	handle_export_assignment(t_shell *shell, char *arg,
							char *equal, int key_len)
{
	char	*key;
	char	*val;

	if (key_len == 0)
		return (fprintf(stderr,
				"minishell: export: `%s': not a valid identifier\n", arg), 1);
	key = ft_strndup(arg, key_len);
	if (!key)
		return (perror("minishell: export: malloc"), 1);
	if (!is_valid_env_key(key))
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", key);
		free(key);
		return (1);
	}
	val = ft_strdup(equal + 1);
	if (!val)
		return (perror("minishell: export: malloc"), free(key), 1);
	if (set_env_variable(&(shell->env), key, val) != 0)
		return (free(key), free(val), 1);
	free(key);
	free(val);
	return (0);
}


int	process_export_argument(t_shell *shell, char *arg)
{
	char	*equal;
	int		key_len;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key_len = equal - arg;
		return (handle_export_assignment(shell, arg, equal, key_len));
	}
	if (!is_valid_env_key(arg))
	{
		fprintf(stderr,
			"minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	return (0);
}


int	handle_export_with_args(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	overall_status;

	i = 1;
	overall_status = 0;
	while (cmd->args[i] != NULL)
	{
		if (process_export_argument(shell, cmd->args[i]) != 0)
			overall_status = 1;
		i++;
	}
	return (overall_status);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	if (cmd->args[1] == NULL)
    {
        print_export(shell->env);
		return 0;
    }
	return (handle_export_with_args(shell, cmd));
}

