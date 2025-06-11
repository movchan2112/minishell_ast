#include "minishell.h"

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return env->val;
        env = env->next;
    }
    return NULL;
}

t_env *find_env_key(t_env *env, const char *key)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return env;
		env = env->next;
	}
	return NULL;
}
void	ft_free_env(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env->next;

		if (env->key)
			free(env->key);
		if (env->val)
			free(env->val);
		free(env);

		env = tmp;
	}
}


/*
** Main function to change an environment variable's value or add a new one.
** Adheres to norminette standards by delegating tasks to helper functions.
*/
void	change_env_value(t_env **env_list_ptr, char *key, char *value)
{
	t_env	*new_node;

	if (!env_list_ptr || !key)
		return ;

	// Attempt to update an existing variable
	if (update_existing_env_value(*env_list_ptr, key, value) == 0)
		return ; // Successfully updated

	// If not found, create and add a new variable
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return ; // Error handled in create_new_env_node

	add_new_env_node_to_list(env_list_ptr, new_node);
}