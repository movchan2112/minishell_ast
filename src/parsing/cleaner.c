#include "minishell.h"

void	free_token_list(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = tmp;
	}
}

void free_ast(t_ast *node)
{
	if (!node)
		return;

	free_ast(node->left);
	free_ast(node->right);

	if (node->cmd)
	{
		free(node->cmd->cmd);
		if (node->cmd->args)
		{
			for (int i = 0; node->cmd->args[i]; i++)
				free(node->cmd->args[i]);
			free(node->cmd->args);
		}
		free(node->cmd->infile);
		free(node->cmd->outfile);
		free(node->cmd->heredoc);
		free(node->cmd);
	}
	free(node);
}
