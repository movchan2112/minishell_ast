#include "minishell.h"

int ft_echo(t_shell *shell,t_cmd *cmd)
{
	int n = 1;
	int i = 0;
	if(cmd->args[1])
		if(ft_strcmp(cmd->args[1],"-n") == 0)
		{
			i = 1;
			n++;
		}
		else if(ft_strcmp(cmd->args[1],"$?\n") == 0)
		{
			i = 1;
			n++;
			printf("%d",shell->exit_status);
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
	return 0;
}

void	ft_env(t_shell *shell)
{
	t_env *env = shell->env;
	while (env)
	{
		if (env->val)
			printf("%s=%s\n", env->key, env->val);
			env = env->next;
	}
}

/// FOR CD

int	ft_pwd()
{
	char cwd[100];

	if(getcwd(cwd,sizeof(cwd)))
	{
		printf("%s\n",cwd);
		return 1;
	}
	else
	{
		perror("ERROR OF PWD");
		return 0;
	}
}
void print_export(t_env *env1)
{
	t_env *env = env1;
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->val)
			printf("=\"%s\"", env->val);
		printf("\n");
		env = env->next;
	}
}

void	ft_export(t_shell *shell, t_cmd *cmd)
{
	if(cmd->args[1])
	{
		char *eq = ft_strchr(cmd->args[1], '=');
		if (!eq)
			return; // nothing to assign
	
		int key_len = eq - cmd->args[1];
		char *key = ft_strndup(cmd->args[1], key_len);
		char *val = ft_strdup(eq + 1);
	
		t_env *existing = find_env_key(shell->env, key);
		if (existing)
		{
			free(existing->val);
			existing->val = val;  // update value
		}
		else
		{
			t_env *new = new_env_node(key, val);
			add_env_node(&shell->env,key,val);
			free(val); // new_env_node duplicates it again
		}
		free(key);
	}
	else
		print_export(shell->env);
}

void cleanup_shell(t_shell *shell)
{
    free_token_list(shell->tokens);
    free_ast(shell->ast);
    ft_free_env(shell->env);
}

int ft_exit(t_shell *shell)
{
	cleanup_shell(shell);
    exit(shell->exit_status);
}

void ft_unset(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args[1])
		return;

	t_env *curr = shell->env;
	t_env *prev = NULL;

	while (curr)
	{
		if (strcmp(curr->key, cmd->args[1]) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env = curr->next;

			free(curr->key);
			free(curr->val);
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_cd(t_shell *shell, t_cmd *cmd)
{
	char *oldpwd = getcwd(NULL,0);

	const char *target;

	if(!cmd->args[1])
		target = get_env_value(shell->env,"HOME");
	else if(ft_strcmp(cmd->args[1],"-") == 0)
		target = get_env_value(shell->env,"OLDPWD");
	else
		target = cmd->args[1];
	
	if(!target || chdir(target) != 0)
	{
		perror("cd");
		return 1;
	}
	
	char *newpwd = getcwd(NULL,0);

	change_env_value(shell->env,"OLDPWD",oldpwd);
	change_env_value(shell->env,"PWD",newpwd);
	
	free(newpwd);
	free(oldpwd);
	return 0;
}