#include "../inc/minishell.h"

t_token	*copy_token_node(const t_token *orig)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		perror("minishell: malloc for token copy failed");
		return (NULL);
	}
	new->value = ft_strdup(orig->value);
	if (!new->value)
	{
		perror("minishell: ft_strdup for token copy value failed");
		free(new);
		return (NULL);
	}
	new->type = orig->type;
	new->next = NULL;
	return (new);
}
/* append_token_node.c */
static void	append_token_node(t_token *node,
				t_token **head,
				t_token **tail)
{
	if (!*head)
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*tail)->next = node;
		*tail = node;
	}
}
/* create_and_add_token_copy.c */
static t_token	*create_and_add_token_copy(t_token *original,
						t_token **new_head,
						t_token **new_tail)
{
	t_token	*node;

	node = copy_token_node(original);
	if (!node)
		return (NULL);
	append_token_node(node, new_head, new_tail);
	return (node);
}


t_token	*build_token_list_excluding_two(t_token *start, t_token *end,
	t_token *exclude1, t_token *exclude2)
{
	t_token	*new_head;
	t_token	*new_tail;
	t_token	*current;

	new_head = NULL;
	new_tail = NULL;
	current = start;
	while (current != NULL && current != end)
	{
		if (current == exclude1 || current == exclude2)
		{
			current = current->next;
			continue ;
		}
		if (!create_and_add_token_copy(current, &new_head, &new_tail))
		{
			free_token_list(new_head);
			return (NULL);
		}
		current = current->next;
	}
	return (new_head);
}
/* create_redir_node.c */
t_ast	*create_redir_node_from_token(t_token *op_t)
{
	t_node_type	type;
	t_ast		*node;

	type = get_redir_node_type_from_token(op_t->type);
	if (type == (t_node_type)-1)
		return (NULL);
	node = create_ast_node(type);
	if (!node)
		return (NULL);
	node = init_redir_node_cmd_fields(node);
	if (!node)
		return (NULL);
	return (node);
}