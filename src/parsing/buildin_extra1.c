#include "minishell.h"

int is_valid_env_key(const char *key)
{
    if (!key || *key == '\0') // Cannot be NULL or empty
        return (0);
    if (!ft_isalpha(*key) && *key != '_') // Must start with letter or underscore
        return (0);
    key++;
    while (*key)
    {
        if (!ft_isalnum(*key) && *key != '_') // Subsequent chars must be alphanumeric or _
            return (0);
        key++;
    }
    return (1);
}


int	update_existing_value(t_env *node, const char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
	{
		perror("minishell: export: malloc");
		return (1);
	}
	free(node->val);
	node->val = new_value;
	return (0);
}

int	dup_key_value(const char *key, const char *value,
			char **key_dup, char **value_dup)
{
	*key_dup = ft_strdup(key);
	if (!*key_dup)
	{
		perror("minishell: export: malloc");
		return (0);
	}
	*value_dup = ft_strdup(value);
	if (!*value_dup)
	{
		perror("minishell: export: malloc");
		free(*key_dup);
		return (0);
	}
	return (1);
}
t_env	*create_new_node(const char *key, const char *value)
{
	t_env	*new;
	char	*key_dup;
	char	*value_dup;

	if (!dup_key_value(key, value, &key_dup, &value_dup))
		return (NULL);
	new = malloc(sizeof(t_env));
	if (!new)
	{
		perror("minishell: export: malloc");
		free(key_dup);
		free(value_dup);
		return (NULL);
	}
	new->key = key_dup;
	new->val = value_dup;
	new->next = NULL;
	return (new);
}

int	set_env_variable(t_env **env_list_ptr, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*prev;
	t_env	*new;

	cur = *env_list_ptr;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (update_existing_value(cur, value));
		prev = cur;
		cur = cur->next;
	}
	new = create_new_node(key, value);
	if (!new)
		return (1);
	if (prev == NULL)
		*env_list_ptr = new;
	else
		prev->next = new;
	return (0);
}