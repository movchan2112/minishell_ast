/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:07:39 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 19:49:46 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value != NULL)
		node->val = ft_strdup(value);
	else
		node->val = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env_list, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = new_env_node(key, value);
	if (!new_node)
		return ;
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}


/**
* @brief Processes a single environment string (e.g., "KEY=value")
* and adds it to the environment list.
*
* @param env_str The environment string to process.
* @param head Pointer to the head of the environment list.
*/
static void	process_env_entry(char *env_str, t_env **head)
{
	char	*delimiter_pos;
	char	*key;
	char	*val;
	int		key_len;

	delimiter_pos = ft_strchr(env_str, '=');
	if (delimiter_pos)
	{
		key_len = delimiter_pos - env_str;
		key = ft_strndup(env_str, key_len);
		if (!key)
		{
			free(key);
			return ;
		}
		val = ft_strdup(delimiter_pos + 1);
		if (!val)
		{
			free(key);
			return ;
		}
		add_env_node(head, key, val);
		free(key);
		free(val);
	}
}

/**
* @brief Initializes the environment list from an array of strings.
* Each string is expected to be in the format "KEY=value".
*
* @param envp Array of environment strings.
* @return Pointer to the head of the created environment list.
*/
t_env	*init_env(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		process_env_entry(envp[i], &head);
		i++;
	}
	return (head);
}
