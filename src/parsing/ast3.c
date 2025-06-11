#include "../inc/minishell.h"

int	handle_redir_if_token(t_token *cur,
				t_token **op, t_token **file)
{
	if (cur->type == TOKEN_REDIR_IN
	|| cur->type == TOKEN_REDIR_OUT
	|| cur->type == TOKEN_REDIR_APPEND
	|| cur->type == TOKEN_HEREDOC)
	{
		if (cur->next && cur->next->type == TOKEN_WORD)
		{
			*op   = cur;
			*file = cur->next;
		}
		else
		{
			fprintf(stderr,
				"minishell: syntax error near token `%s'\n",
				cur->value);
			*op   = NULL;
			*file = NULL;
			return (0);
		}
	}
	return (1);
}


void	find_last_redir_info_in_segment(t_token *start,
				t_token *end,
				t_token **op_token_ptr,
				t_token **file_token_ptr)
{
	t_token	*current;

	*op_token_ptr   = NULL;
	*file_token_ptr = NULL;
	current = start;
	while (current && current != end)
	{
		if (!handle_redir_if_token(current,
				op_token_ptr, file_token_ptr))
			return ;
		current = current->next;
	}
}


t_ast	*init_redir_node_cmd_fields(t_ast *node)
{
	node->cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
		free(node);
		perror("minishell: malloc for redir t_cmd");
		return (NULL);
	}
	node->cmd->cmd = NULL;
	node->cmd->args = NULL;
	node->cmd->outfile = NULL;
	node->cmd->append = 0;
	node->cmd->infile = NULL;
	node->cmd->heredoc = NULL;
	return (node);
}

t_node_type	get_redir_node_type_from_token(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	if (token_type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	if (token_type == TOKEN_REDIR_APPEND)
		return (NODE_REDIR_APPEND);
	if (token_type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return ((t_node_type)-1);
}

t_ast	*process_pipe_segment(t_token *s, \
	t_token *e, t_token *p_tok)
{
	t_ast	*node;
	t_ast	*left_ast;
	t_ast	*right_ast;

	node = create_ast_node(NODE_PIPE);
	if (!node)
		return (NULL);
	left_ast = build_ast_from_tokens(s, p_tok);
	right_ast = build_ast_from_tokens(p_tok->next, e);
	if (!left_ast || !right_ast)
	{
		if ((!left_ast && p_tok != s) || (!right_ast && p_tok->next != e))
			fprintf(stderr, "minishell: syntax error near `|'\n");
		if (left_ast)
			free(left_ast); /* TODO: Implement free_ast */
		if (right_ast)
			free(right_ast);/* TODO: Implement free_ast */
		free(node);
		return (NULL);
	}
	node->left = left_ast;
	node->right = right_ast;
	return (node);
}