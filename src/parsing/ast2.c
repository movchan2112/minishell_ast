#include "../inc/minishell.h"

t_ast	*create_ast_node(t_node_type type)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
	{
		perror("minishell: malloc failed for ast node");
		return (NULL);
	}
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}

t_ast	*create_cmd_node(char *cmd_name_allocated, char **args_allocated)
{
	t_ast	*node;

	node = create_ast_node(NODE_CMD);
	if (!node)
		return (NULL);
	node->cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
		perror("minishell: malloc failed for t_cmd");
		free(node);
		return (NULL);
	}
	node->cmd->cmd = cmd_name_allocated;
	node->cmd->args = args_allocated;
	node->cmd->outfile = NULL;
	node->cmd->append = 0;
	node->cmd->infile = NULL;
	node->cmd->heredoc = NULL;
	return (node);
}

int	prepare_cmd_components(t_token *start, t_token *end,
	char **cmd_name_dup, char ***args_array)
{
	t_args_builder	bs;
	int				word_c;

	if (!start || start == end)
		return (1);
	word_c = count_words_in_segment(start, end);
	if (word_c == 0)
		return (1);
	*args_array = populate_args_array(start, end, word_c, &bs);
	if (!*args_array)
		return (1);
	if (!bs.cmd_name_val)
		return (fprintf(stderr,
			"minishell: internal error - cmd_name_val is NULL\n"),
			free_argv_array(*args_array), 1);
	*cmd_name_dup = ft_strdup(bs.cmd_name_val);
	if (!*cmd_name_dup)
		return (perror("minishell: ft_strdup failed"),
			free_argv_array(*args_array), 1);
	return (0);
}

t_ast	*create_cmd_node_from_tokens(t_token *start, t_token *end)
{
	char	*cmd_name_dup;
	char	**args_array;
	t_ast	*new_node;

	if (prepare_cmd_components(start, end, &cmd_name_dup, &args_array))
		return (NULL);
	new_node = create_cmd_node(cmd_name_dup, args_array);
	if (!new_node)
	{
		free(cmd_name_dup);
		free_argv_array(args_array);
		return (NULL);
	}
	return (new_node);
}

t_token	*find_last_pipe_token_in_segment(t_token *start, \
	t_token *end)
{
	t_token	*current;
	t_token	*pipe_token;

	pipe_token = NULL;
	current = start;
	while (current != NULL && current != end)
	{
		if (current->type == TOKEN_PIPE)
			pipe_token = current;
		current = current->next;
	}
	return (pipe_token);
}