#include "../inc/minishell.h"

int exec_tree(t_shell *shell, t_ast *ast)
{
	int status = 0;

	if (!ast)
		return (0);
	if (ast->type == NODE_PIPE)
		status = exec_pipe_node(shell, ast);
	else if (ast->type == NODE_REDIR_OUT || ast->type == NODE_REDIR_APPEND
		|| ast->type == NODE_REDIR_IN || ast->type == NODE_HEREDOC)
		status = exec_redir_chain(shell, ast);
	else if (ast->type == NODE_CMD)
		status = exec_cmd_node(shell, ast);
	else
	{
		write_error("minishell: unknown AST node type", NULL, NULL);
		status = EXIT_FAILURE;
	}

	shell->exit_status = status;
	return (status);
}

void free_shell(t_shell *shell)
{
	free_command_resources(shell);
	ft_free_env(shell->env);
}


void	exec_child_left(t_shell *shell, t_ast *ast, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	shell->exit_status = exec_tree(shell, ast->left);
	free_shell(shell);
	exit(shell->exit_status);
}

void	exec_child_right(t_shell *shell, t_ast *ast, int *pipefd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	shell->exit_status = exec_tree(shell, ast->right);
	free_shell(shell);
	exit(shell->exit_status);
}

void	update_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	else
		shell->exit_status = EXIT_FAILURE;
}