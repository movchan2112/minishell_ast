#include "minishell.h"

char	*ft_strdup_checked(char *str, char *err_msg)
{
	char	*dup;

	if (!str)
		return (NULL); // Return NULL if input is NULL (for empty values)

	dup = ft_strdup(str);
	if (!dup)
		perror(err_msg);
	return (dup);
}

/*
** Helper function to handle value duplication within create_new_env_node.
** Returns NULL on failure to duplicate, freeing key_to_free if necessary.
*/
char	*get_new_val_dup(char *value, char *key_to_free)
{
	char	*new_val_dup;

	new_val_dup = NULL;
	if (value != NULL)
	{
		new_val_dup = ft_strdup_checked(value,
				"minishell: create_new_node: ft_strdup for value failed");
		if (!new_val_dup)
		{
			free(key_to_free);
			return (NULL);
		}
	}
	return (new_val_dup);
}

/*
** Helper function to create a new t_env node.
** Handles memory allocation and string duplication for key and value.
** Returns NULL on allocation or duplication failure.
*/
t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;
	char	*new_key_dup;
	char	*new_val_dup;

	new_key_dup = ft_strdup_checked(key,
			"minishell: create_new_node: ft_strdup for key failed");
	if (!new_key_dup)
		return (NULL);

	new_val_dup = get_new_val_dup(value, new_key_dup);
	if (value != NULL && !new_val_dup) // Check if value was non-NULL and duplication failed
		return (NULL);

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("minishell: create_new_node: malloc failed");
		free(new_key_dup);
		free(new_val_dup); // new_val_dup might be NULL, free(NULL) is safe
		return (NULL);
	}
	new_node->key = new_key_dup;
	new_node->val = new_val_dup;
	new_node->next = NULL;
	return (new_node);
}

/*
** Helper function to update the value of an existing environment variable.
** Returns 0 on success (key found and updated), 1 if key not found or update fails.
*/
int	update_existing_env_value(t_env *env_list, char *key, char *value)
{
	t_env	*current;
	char	*new_val_dup;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			new_val_dup = NULL;
			if (value != NULL)
			{
				new_val_dup = ft_strdup_checked(value,
						"minishell: update_env: ft_strdup for value failed");
				if (!new_val_dup)
					return (1); // Indicate failure to duplicate
			}
			free(current->val);
			current->val = new_val_dup;
			return (0); // Successfully updated
		}
		current = current->next;
	}
	return (1); // Key not found in the list
}

/*
** Helper function to add a new node to the end of the environment list.
*/
void	add_new_env_node_to_list(t_env **env_list_ptr, t_env *new_node)
{
	t_env	*current;

	if (!new_node)
		return ; // Should not happen if create_new_env_node handles errors

	if (*env_list_ptr == NULL)
	{
		*env_list_ptr = new_node;
	}
	else
	{
		current = *env_list_ptr;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

