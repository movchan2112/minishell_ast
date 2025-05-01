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

void	free_ast(t_ast *node)
{
	if (!node)
		return;

	// Рекурсивно освобождаем левую и правую стороны
	free_ast(node->left);
	free_ast(node->right);

	// Освобождаем содержимое команды, если есть
	if (node->type == NODE_CMD && node->cmd)
	{
		if (node->cmd->cmd)
			free(node->cmd->cmd);

		if (node->cmd->args)
		{
			int i = 0;
			while (node->cmd->args[i])
				free(node->cmd->args[i++]);
			free(node->cmd->args);
		}

		if (node->cmd->infile)
			free(node->cmd->infile);
		if (node->cmd->outfile)
			free(node->cmd->outfile);
		if (node->cmd->heredoc)
			free(node->cmd->heredoc);

		free(node->cmd);
	}

	free(node);
}
