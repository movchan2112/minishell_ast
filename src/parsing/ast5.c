#include "../inc/minishell.h"

int	set_redir_target(t_ast *node, t_token *file_t)
{
	char	*dup;

	dup = ft_strdup(file_t->value);
	if (!dup)
	{
		free(node->cmd);
		free(node);
		perror("minishell: strdup");
		return (1);
	}
	if (node->type == NODE_REDIR_IN)
		node->cmd->infile = dup;
	else if (node->type == NODE_REDIR_OUT)
		node->cmd->outfile = dup;
	else if (node->type == NODE_REDIR_APPEND)
	{
		node->cmd->outfile = dup;
		node->cmd->append = 1;
	}
	else if (node->type == NODE_HEREDOC)
		node->cmd->heredoc = dup;
	return (0);
}

/* attach_redir_child.c */
int	attach_redir_child(t_ast *node,
				t_token *s, t_token *e,
				t_token *op_t, t_token *file_t)
{
	t_token	*cmd_tokens;
	t_ast	*child;

	cmd_tokens = build_token_list_excluding_two(s, e, op_t, file_t);
	child = build_ast_from_tokens(cmd_tokens, NULL);
	free_token_list(cmd_tokens);
	if (!child)
	{
		fprintf(stderr,
			"minishell: syntax error in command for redirection\n");
		return (1);
	}
	node->left = child;
	return (0);
}

/* process_redir_segment.c */
t_ast	*process_redir_segment(t_token *s, t_token *e,
				t_token *op_t, t_token *file_t)
{
	t_ast	*node;

	node = create_redir_node_from_token(op_t);
	if (!node)
		return (NULL);
	if (set_redir_target(node, file_t))
		return (NULL);
	if (attach_redir_child(node, s, e, op_t, file_t))
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

t_ast	*build_ast_from_tokens(t_token *start, t_token *end)
{
	t_token	*op_token;
	t_token	*file_or_delim_token;

	if (!start || start == end)
		return (NULL);
	op_token = find_last_pipe_token_in_segment(start, end);
	if (op_token)
		return (process_pipe_segment(start, end, op_token));
	file_or_delim_token = NULL;
	find_last_redir_info_in_segment(start, end, \
		&op_token, &file_or_delim_token);
	if (op_token && file_or_delim_token)
		return (process_redir_segment(start, end, \
			op_token, file_or_delim_token));
	else if (op_token && !file_or_delim_token)
		return (NULL);
	return (create_cmd_node_from_tokens(start, end));
}


void write_error(char *s1, char *s2, char *s3)
{
    if (s1)
        write(STDERR_FILENO, s1, ft_strlen(s1));
    if (s2)
        write(STDERR_FILENO, s2, ft_strlen(s2));
    if (s3)
        write(STDERR_FILENO, s3, ft_strlen(s3));
    write(STDERR_FILENO, "\n", 1);
}
